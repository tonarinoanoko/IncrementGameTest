import os

# このファイルから見て2つ上の階層がProjectRoot
BASE_DIR = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

def get_root_path(rel_path):
    return os.path.normpath(os.path.join(BASE_DIR, rel_path))

# よく使うパス
CSV_DIR = get_root_path("Masters")
CPP_DIR = get_root_path("Source/MasterData/Generated")
JSON_DIR = get_root_path("Resources/Data")