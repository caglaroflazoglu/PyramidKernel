import fileinput
import json
import re
from contextlib import contextmanager, suppress
from dataclasses import dataclass
from enum import Enum, auto
from io import StringIO
from pathlib import Path
from types import SimpleNamespace
from typing import Match


class MetadataError(Exception):
    pass


class Annotation(Enum):
    COMMAND = auto()
    HANDLER = auto()


@dataclass
class Metadata:
    file: Path
    match: Match
    annotation: Annotation
    metadata: SimpleNamespace

    @classmethod
    def from_match(cls, path, match, data):
        if not hasattr(data, "annotation"):
            raise MetadataError("Annotation must be in metadata")

        try:
            annotation = getattr(Annotation, data.annotation.upper())
        except AttributeError as exc:
            raise MetadataError(
                f"Annotation must be one of these: (command, handler) not {data.annotation}"
            ) from exc

        return cls(path, match, annotation, data)


GET_COMMENTS = re.compile(r"(?s)\/\*\s.*?\*\/")


def parse(path):
    with open(path) as f:
        content = f.read()

    metadatas = []
    for match in re.finditer(GET_COMMENTS, content):
        data = match.group(0).strip("/*").strip()
        with suppress(ValueError):
            data = json.loads(data, object_hook=lambda obj: SimpleNamespace(**obj))
            metadata = Metadata.from_match(path, match, data)
            metadatas.append(metadata)
    return metadatas


class Code(StringIO):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.indentlevel = 2

    def nl(self):
        self.write("\n")
        self.write(self.indentlevel * " " * 3)

    @contextmanager
    def comp(self, command, req_else=True):
        self.nl()
        self.write(f"{'else ' if req_else else ''}if(!strcmp(command, \"{command}\"))")
        try:
            self.write("{")
            self.indentlevel += 1
            yield
        finally:
            self.indentlevel -= 1
            self.nl()
            self.write("}")


GEN_START = "/* start of generated code */"
GEN_END = "/* end of generated code */"


def generate_handler(commands, help_message):
    handler = Code()
    handler.write(handler.indentlevel * " " * 3)
    handler.write(GEN_START)
    with handler.comp("help", req_else=False):
        for message in help_message:
            handler.nl()
            handler.write(message)
    for command in commands:
        with handler.comp(command.metadata.command):
            handler.nl()
            handler.write(f"{command.metadata.command}();")
    handler.nl()
    handler.write(GEN_END)
    return handler


def main(start_path):
    path = Path(start_path)
    handlers, commands = [], []
    for source in path.rglob("*.c"):
        results = parse(source)
        for result in results:
            locals()[f"{result.metadata.annotation.lower()}s"].append(result)

    help_message = (
        r'printf("\n{command.metadata.command}: {command.metadata.help}");'.format(command = command)
        for command in commands
    )
    handler_impl = generate_handler(commands, help_message).getvalue()
    for handler in handlers:
        with open(handler.file) as f:
            contents = f.readlines()
        try:
            _buf = list(content.strip() for content in contents)
            start = _buf.index(GEN_START)
            end = _buf.index(GEN_END)
        except ValueError as exc:
            raise MetadataError(
                f"Couldn't find any generation starter mark in {handler.file}"
            ) from exc

        for _ in range(start, end + 1):
            contents.pop(start)

        counter = start
        for line in handler_impl.split("\n"):
            contents.insert(counter, line + "\n")
            counter += 1

        with open(handler.file, "w") as f:
            f.writelines(contents)


if __name__ == "__main__":
    import sys

    main(*sys.argv[1:])
