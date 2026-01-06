import hashlib
import json
import subprocess
import sys
from pathlib import Path
from typing import Any

ROOT_PATH = Path(__file__).parent
SRC_PATH = ROOT_PATH / "src"
BUILD_PATH = ROOT_PATH / "build"
OBJ_PATH = BUILD_PATH / "obj"
HASH_PATH = BUILD_PATH / "hash.json"
EXECUTABLE_PATH = BUILD_PATH / "chat"


class JsonReader:
    @staticmethod
    def write(obj: Any, path: Path) -> None:
        with open(path, mode="w", encoding="utf-8") as json_file:
            json.dump(obj, json_file, indent=4)

    @staticmethod
    def read(path: Path) -> Any:
        if not path.exists():
            return {}

        with open(path, encoding="utf-8") as json_file:
            file = json.load(json_file)

        return file


def get_file_hash(file: Path) -> str:
    buf_size = 65536
    sha512 = hashlib.sha512()

    with open(file, "rb") as f:
        while True:
            data = f.read(buf_size)
            if not data:
                break
            sha512.update(data)

    return sha512.hexdigest()


def include_sources(sources: dict[str, Path], dir: Path) -> dict[str, Path]:
    for entity in dir.iterdir():
        if entity.is_dir():
            include_sources(sources, entity)
        elif entity.is_file() and entity.suffix == ".cpp":
            encoded_path = str(entity).encode()
            hash = hashlib.sha512(encoded_path)
            sources[hash.hexdigest()] = entity

    return sources


def assemble_sources(sources: dict[str, Path]) -> int:
    changes = 0
    file_hashes: dict[str, str] = JsonReader.read(HASH_PATH)
    for hash, path in sources.items():
        file_hash = file_hashes.get(hash)
        current_file_hash = get_file_hash(path)
        if file_hash is None or file_hash != current_file_hash:
            file_hashes[hash] = current_file_hash
            obj_path = OBJ_PATH / f"{hash}.o"
            subprocess.run(["g++", "-std=c++23", "-c", path, "-o", obj_path])
            changes += 1

    JsonReader.write(file_hashes, HASH_PATH)

    return changes


def link_objects(sources: dict[str, Path], changes: int) -> None:
    if changes == 0:
        return None

    objects: list[Path] = []
    for hash in sources.keys():
        obj_path = OBJ_PATH / f"{hash}.o"
        objects.append(obj_path)

    subprocess.run(["g++", *objects, "-o", EXECUTABLE_PATH])


def run(args: list[str]) -> None:
    subprocess.run([EXECUTABLE_PATH, *args])


def build() -> None:
    sources = include_sources({}, SRC_PATH)

    BUILD_PATH.mkdir(exist_ok=True)
    OBJ_PATH.mkdir(exist_ok=True)

    changes = assemble_sources(sources)
    link_objects(sources, changes)

    args = sys.argv[1:] if len(sys.argv) > 1 else []

    try:
        run(args)
    except FileNotFoundError:
        print("Error: executable file was not found due to error during compilation")


if __name__ == "__main__":
    build()
