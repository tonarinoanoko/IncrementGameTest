import tkinter as tk
from tkinter import ttk, messagebox
import sys
import os
import csv

# 共通モジュールのパスを通す
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from common.path_utils import CSV_DIR
from common.google_auth import get_sheets_service

class SheetDownloader(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("スプレッドシート・ダウンローダー")
        self.geometry("700x550")
        
        # 前回使用したIDを保存するファイルのパス
        self.config_file = os.path.join(os.path.dirname(__file__), "last_id.txt")
        
        self.service = None
        self.master_list = []  # 管理シートから取得したマスタ情報のリスト
        
        self._setup_gui()
        self._load_last_id()  # 起動時に前回IDを読み込み

    def _setup_gui(self):
        """GUIのレイアウト構築"""
        main_frame = ttk.Frame(self, padding="10")
        main_frame.pack(fill=tk.BOTH, expand=True)

        # --- 上部：管理シート設定 ---
        top_frame = ttk.LabelFrame(main_frame, text="管理シート設定", padding="5")
        top_frame.pack(fill=tk.X, pady=5)

        ttk.Label(top_frame, text="管理シートID:").pack(side=tk.LEFT, padx=5)
        self.mgmt_id_entry = ttk.Entry(top_frame)
        self.mgmt_id_entry.pack(side=tk.LEFT, fill=tk.X, expand=True, padx=5)

        ttk.Button(top_frame, text="リスト取得", command=self.load_management_list).pack(side=tk.LEFT, padx=5)

        # --- 中部：シート選択リスト ---
        mid_frame = ttk.LabelFrame(main_frame, text="ダウンロード対象の選択", padding="5")
        mid_frame.pack(fill=tk.BOTH, expand=True, pady=5)

        list_inner_frame = ttk.Frame(mid_frame)
        list_inner_frame.pack(fill=tk.BOTH, expand=True)

        self.scrollbar = ttk.Scrollbar(list_inner_frame)
        self.scrollbar.pack(side=tk.RIGHT, fill=tk.Y)

        self.sheet_listbox = tk.Listbox(
            list_inner_frame, 
            selectmode=tk.MULTIPLE, 
            yscrollcommand=self.scrollbar.set,
            font=("MS Gothic", 10)
        )
        self.sheet_listbox.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        self.scrollbar.config(command=self.sheet_listbox.yview)

        # --- 下部：実行ボタンとログ ---
        bottom_frame = ttk.Frame(main_frame)
        bottom_frame.pack(fill=tk.X, pady=5)

        ttk.Button(bottom_frame, text="選択項目をダウンロード", command=self.download_selected).pack(side=tk.LEFT, padx=5)
        ttk.Button(bottom_frame, text="すべてダウンロード", command=self.download_all).pack(side=tk.LEFT, padx=5)
        ttk.Button(bottom_frame, text="ログ消去", command=self.clear_log).pack(side=tk.RIGHT, padx=5)

        self.log_text = tk.Text(main_frame, height=12, state=tk.DISABLED, font=("MS Gothic", 9), bg="#f0f0f0")
        self.log_text.pack(fill=tk.BOTH, expand=True, pady=5)

    # --- IDの保存・読み込み処理 ---
    def _load_last_id(self):
        """ファイルから前回のIDを読み取ってセットする"""
        if os.path.exists(self.config_file):
            try:
                with open(self.config_file, "r", encoding="utf-8") as f:
                    last_id = f.read().strip()
                    if last_id:
                        self.mgmt_id_entry.insert(0, last_id)
            except Exception:
                pass

    def _save_last_id(self, sheet_id):
        """現在のIDをファイルに保存する"""
        try:
            with open(self.config_file, "w", encoding="utf-8") as f:
                f.write(sheet_id)
        except Exception as e:
            self.log(f"IDの保存に失敗しました: {e}")

    # --- ログ操作 ---
    def log(self, message):
        self.log_text.config(state=tk.NORMAL)
        self.log_text.insert(tk.END, f"> {message}\n")
        self.log_text.see(tk.END)
        self.log_text.config(state=tk.DISABLED)

    def clear_log(self):
        self.log_text.config(state=tk.NORMAL)
        self.log_text.delete('1.0', tk.END)
        self.log_text.config(state=tk.DISABLED)

    # --- API連携ロジック ---
    def load_management_list(self):
        """管理用シートを読み込んでリストボックスを更新する"""
        sheet_id = self.mgmt_id_entry.get().strip()
        if not sheet_id:
            messagebox.showwarning("警告", "管理シートIDを入力してください。")
            return

        # IDを保存
        self._save_last_id(sheet_id)

        self.log("Google Sheets APIに接続中...")
        try:
            if not self.service:
                self.service = get_sheets_service()
            
            # 管理シートの「A2:D100」範囲を取得
            range_name = 'A2:D100'
            result = self.service.spreadsheets().values().get(
                spreadsheetId=sheet_id, range=range_name).execute()
            rows = result.get('values', [])

            if not rows:
                self.log("管理シートにデータが見つかりませんでした。")
                return

            self.master_list = []
            self.sheet_listbox.delete(0, tk.END)

            for row in rows:
                if len(row) < 4: continue
                info = {
                    "display_name": row[0],
                    "spreadsheet_id": row[1],
                    "tab_name": row[2],
                    "file_name": row[3]
                }
                self.master_list.append(info)
                self.sheet_listbox.insert(tk.END, f"{info['display_name']} ({info['file_name']})")

            self.log(f"{len(self.master_list)} 件のマスタ定義を読み込みました。")

        except Exception as e:
            self.log(f"エラー: {str(e)}")
            messagebox.showerror("エラー", f"管理シートの読み込みに失敗しました:\n{e}")

    def download_selected(self):
        indices = self.sheet_listbox.curselection()
        if not indices:
            messagebox.showwarning("警告", "ダウンロードする項目を選択してください。")
            return
        
        for i in indices:
            self._execute_download(self.master_list[i])

    def download_all(self):
        if not self.master_list:
            self.log("リストが空です。先に管理シートを読み込んでください。")
            return
        for info in self.master_list:
            self._execute_download(info)

    def _execute_download(self, info):
        """特定のスプレッドシートをCSVとして保存する"""
        try:
            self.log(f"開始: {info['display_name']} ...")
            
            # API経由で各シートの全データを取得
            range_name = f"{info['tab_name']}!A1:Z1000"
            result = self.service.spreadsheets().values().get(
                spreadsheetId=info['spreadsheet_id'], range=range_name).execute()
            values = result.get('values', [])

            if not values:
                self.log(f"  [スキップ] データがありません: {info['display_name']}")
                return

            # CSVとして保存
            os.makedirs(CSV_DIR, exist_ok=True)
            output_path = os.path.join(CSV_DIR, info['file_name'])
            
            with open(output_path, 'w', encoding='utf-8-sig', newline='') as f:
                writer = csv.writer(f)
                writer.writerows(values)
            
            self.log(f"  [完了] 保存先: {output_path}")

        except Exception as e:
            self.log(f"  [エラー] {info['display_name']} のDL失敗: {e}")

if __name__ == "__main__":
    app = SheetDownloader()
    app.mainloop()