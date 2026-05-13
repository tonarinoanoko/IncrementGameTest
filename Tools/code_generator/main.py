import tkinter as tk
from tkinter import ttk, messagebox
import sys
import os
import csv
import json

# 共通モジュールのインポート設定
CURRENT_DIR = os.path.dirname(os.path.abspath(__file__)) 
TOOLS_DIR = os.path.dirname(CURRENT_DIR)
sys.path.append(TOOLS_DIR)

# プロジェクトの共通ユーティリティをインポート
from common.path_utils import CSV_DIR, get_root_path
from common.google_auth import get_sheets_service

class CodeGenerator(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("マスターデータ・ジェネレーター")
        self.geometry("750x600")
        
        self.config_file = os.path.join(CURRENT_DIR, "last_id.txt")
        self.service = None
        self.master_list = []
        
        self._setup_gui()
        self._load_last_id()

    def _setup_gui(self):
        main_frame = ttk.Frame(self, padding="10")
        main_frame.pack(fill=tk.BOTH, expand=True)

        top_frame = ttk.LabelFrame(main_frame, text="管理シート設定", padding="5")
        top_frame.pack(fill=tk.X, pady=5)
        self.mgmt_id_entry = ttk.Entry(top_frame)
        self.mgmt_id_entry.pack(side=tk.LEFT, fill=tk.X, expand=True, padx=5)
        ttk.Button(top_frame, text="定義を読み込み", command=self.load_definition).pack(side=tk.LEFT, padx=5)

        mid_frame = ttk.LabelFrame(main_frame, text="生成対象（複数選択可）", padding="5")
        mid_frame.pack(fill=tk.BOTH, expand=True, pady=5)
        self.sheet_listbox = tk.Listbox(mid_frame, font=("MS Gothic", 10), selectmode=tk.EXTENDED)
        self.sheet_listbox.pack(fill=tk.BOTH, expand=True)

        btn_frame = ttk.Frame(main_frame)
        btn_frame.pack(fill=tk.X, pady=5)
        ttk.Button(btn_frame, text="選択項目のみ生成", command=self.generate_selected).pack(side=tk.LEFT, padx=5)
        ttk.Button(btn_frame, text="全項目を生成", command=self.generate_all).pack(side=tk.LEFT, padx=5)
        ttk.Button(btn_frame, text="ログ消去", command=self.clear_log).pack(side=tk.RIGHT, padx=5)

        self.log_text = tk.Text(main_frame, height=15, state=tk.DISABLED, font=("MS Gothic", 9), bg="#f0f0f0")
        self.log_text.pack(fill=tk.BOTH, expand=True, pady=5)

    def log(self, message):
        self.log_text.config(state=tk.NORMAL)
        self.log_text.insert(tk.END, f"> {message}\n")
        self.log_text.see(tk.END)
        self.log_text.config(state=tk.DISABLED)

    def clear_log(self):
        self.log_text.config(state=tk.NORMAL)
        self.log_text.delete('1.0', tk.END)
        self.log_text.config(state=tk.DISABLED)

    def _load_last_id(self):
        if os.path.exists(self.config_file):
            try:
                with open(self.config_file, "r", encoding="utf-8") as f:
                    self.mgmt_id_entry.insert(0, f.read().strip())
            except: pass

    def load_definition(self):
        sheet_id = self.mgmt_id_entry.get().strip()
        if not sheet_id: return
        try:
            with open(self.config_file, "w", encoding="utf-8") as f: f.write(sheet_id)
            if not self.service: self.service = get_sheets_service()
            result = self.service.spreadsheets().values().get(spreadsheetId=sheet_id, range='A2:G100').execute()
            rows = result.get('values', [])
            self.master_list = []
            self.sheet_listbox.delete(0, tk.END)
            for r in rows:
                if len(r) < 7: continue
                info = {"name": r[0], "csv": r[3], "class": r[4], "cpp_path": r[5], "json_path": r[6]}
                self.master_list.append(info)
                self.sheet_listbox.insert(tk.END, f"{info['class']} (CSV: {info['csv']})")
            self.log(f"{len(self.master_list)} 件の定義を読み込みました。")
        except Exception as e: self.log(f"定義読み込みエラー: {e}")

    def generate_selected(self):
        selected_indices = self.sheet_listbox.curselection()
        if not selected_indices:
            messagebox.showwarning("警告", "生成する項目を選択してください。")
            return
        for index in selected_indices:
            self._process_master(self.master_list[index])
        self.log("--- 選択項目の生成完了 ---")

    def generate_all(self):
        if not self.master_list: return
        for info in self.master_list:
            self._process_master(info)
        self.log("--- 全項目の生成完了 ---")

    def _process_master(self, info):
        csv_path = os.path.join(CSV_DIR, info["csv"])
        if not os.path.exists(csv_path):
            self.log(f"エラー: CSVが見つかりません {csv_path}")
            return
        try:
            with open(csv_path, 'r', encoding='utf-8-sig') as f:
                reader = csv.reader(f)
                header = next(reader)
                rows = [r for r in list(reader) if r and r[0]]

            fields = []
            for col in header:
                if ':' not in col: continue
                name, type_raw = col.split(':')
                is_enum = (type_raw.lower() == "enum")
                fields.append({"name": name, "type": "string" if is_enum else type_raw.lower(), "is_enum": is_enum})

            # MD_Character -> Character -> ECharacter / IdMax::ECharacter()
            base_name = info['class'].replace("MD_", "")
            enum_type = f"E{base_name}"

            self._generate_enum_header(info, base_name, enum_type, rows)
            self._generate_cpp_header(info, base_name, enum_type, fields)
            self._generate_json_data(info, fields, rows)
        except Exception as e:
            self.log(f"エラー ({info['class']}): {e}")

    def _generate_enum_header(self, info, base_name, enum_type, rows):
        """EClass形式のEnum、ToString、ToEnum、IdMax::EClass()を生成"""
        enum_names = [r[0].strip() for r in rows]
        
        lines = [
            "#pragma once",
            "#include <string>",
            "#include <vector>",
            "#include <unordered_map>",
            "",
            "namespace MyGame {",
            "",
            f"enum class {enum_type} : int {{",
        ]
        for name in enum_names:
            lines.append(f"    {name},")
        lines.append("    Max")
        lines.append("};")
        lines.append("")
        
        lines.append(f"static const std::vector<std::string> {enum_type}Strings = {{")
        for name in enum_names:
            lines.append(f'    "{name}",')
        lines.append("};")
        lines.append("")

        lines.extend([
            f"inline {enum_type} To{enum_type}(const std::string& idStr) {{",
            f"    static const std::unordered_map<std::string, {enum_type}> strToEnum = {{",
        ])
        for name in enum_names:
            lines.append(f'        {{ "{name}", {enum_type}::{name} }},')
        lines.extend([
            "    };",
            "    auto it = strToEnum.find(idStr);",
            f"    return (it != strToEnum.end()) ? it->second : {enum_type}::Max;",
            "}",
            "",
            f"inline std::string ToString({enum_type} id) {{",
            f"    int index = static_cast<int>(id);",
            f"    if (index < 0 || index >= static_cast<int>({enum_type}::Max)) return \"Unknown\";",
            f"    return {enum_type}Strings[index];",
            "}",
            "",
            "struct IdMax {",
            f"    static inline int {enum_type}() {{ return static_cast<int>({enum_type}::Max); }}",
            "};",
            "",
            "}"
        ])

        base_dir = get_root_path(info["cpp_path"].lstrip('\\/'))
        enum_dir = os.path.join(base_dir, "Enums")
        os.makedirs(enum_dir, exist_ok=True)
        
        file_path = os.path.join(enum_dir, f"{enum_type}.h")
        with open(file_path, 'w', encoding='utf-8') as f:
            f.write("\n".join(lines))
        self.log(f"[完了] Enumヘッダー: {file_path}")

    def _generate_cpp_header(self, info, base_name, enum_type, fields):
        """マスタクラスのヘッダー生成"""
        cpp_type_map = {"int": "int", "float": "float", "string": "std::string", "bool": "bool"}
        
        lines = [
            "#pragma once",
            '#include "MD_Base.h"',
            f'#include "Enums/{enum_type}.h"',
            "#include <nlohmann/json.hpp>",
            "#include <unordered_map>",
            "",
            "namespace MyGame {",
            "",
            f"struct {info['class']}Data {{",
        ]
        
        for f in fields:
            t = cpp_type_map.get(f['type'], 'std::string')
            lines.append(f"    {t} {f['name']};")
        
        f_names = [f['name'] for f in fields]
        lines.append("")
        lines.append(f"    NLOHMANN_DEFINE_TYPE_INTRUSIVE({info['class']}Data, {', '.join(f_names)})")
        lines.append("};")
        lines.append("")

        lines.extend([
            f"class {info['class']} : public MD_Base {{",
            "public:",
            "    void load(const nlohmann::json& json) override {",
            "        for(auto& [key, value] : json.items()) {",
            f"            auto data = value.get<{info['class']}Data>();",
            "            _datas[key] = data;",
            "        }",
            "    }",
            "",
            f"    const {info['class']}Data* get(const std::string& id) const {{",
            "        auto it = _datas.find(id);",
            "        return (it != _datas.end()) ? &it->second : nullptr;",
            "    }",
            "",
            f"    const {info['class']}Data* get({enum_type} id) const {{",
            f"        return get(ToString(id));",
            "    }",
            "",
            "private:",
            f"    std::unordered_map<std::string, {info['class']}Data> _datas;",
            "};",
            "}"
        ])

        out_dir = get_root_path(info["cpp_path"].lstrip('\\/'))
        os.makedirs(out_dir, exist_ok=True)
        file_path = os.path.join(out_dir, f"{info['class']}.h")
        with open(file_path, 'w', encoding='utf-8') as f:
            f.write("\n".join(lines))
        self.log(f"[完了] マスタヘッダー: {file_path}")

    def _generate_json_data(self, info, fields, rows):
        """JSONデータ生成"""
        json_data = {}
        for row in rows:
            obj = {}
            key_val = str(row[0])
            for i, f in enumerate(fields):
                if i >= len(row): continue
                val = row[i]
                if f["type"] == "int": val = int(val) if val else 0
                elif f["type"] == "float": val = float(val) if val else 0.0
                elif f["type"] == "bool": val = (val.lower() == "true" or val == "1")
                obj[f["name"]] = val
            json_data[key_val] = obj

        out_dir = get_root_path(info["json_path"].lstrip('\\/'))
        os.makedirs(out_dir, exist_ok=True)
        file_path = os.path.join(out_dir, f"{info['class']}.json")
        with open(file_path, 'w', encoding='utf-8') as f:
            json.dump(json_data, f, indent=4, ensure_ascii=False)
        self.log(f"[完了] JSONデータ: {file_path}")

if __name__ == "__main__":
    CodeGenerator().mainloop()