import os
from google.auth.transport.requests import Request
from google.oauth2.credentials import Credentials
from google_auth_oauthlib.flow import InstalledAppFlow
from googleapiclient.discovery import build

# スコープ設定
SCOPES = ['https://www.googleapis.com/auth/spreadsheets.readonly']

# 共通ファイルパス（このファイル google_auth.py からの相対パスで固定）
CURRENT_DIR = os.path.dirname(os.path.abspath(__file__))
COMMON_CREDS_PATH = os.path.join(CURRENT_DIR, "credentials.json")
COMMON_TOKEN_PATH = os.path.join(CURRENT_DIR, "token.json")

def get_sheets_service():
    """
    共通の credentials.json を使用して認証を行い、サービスを返す。
    どのツールから呼ばれても同じ token.json を使い回す。
    """
    creds = None
    
    # commonディレクトリにあるtoken.jsonを確認
    if os.path.exists(COMMON_TOKEN_PATH):
        creds = Credentials.from_authorized_user_file(COMMON_TOKEN_PATH, SCOPES)
    
    if not creds or not creds.valid:
        if creds and creds.expired and creds.refresh_token:
            creds.refresh(Request())
        else:
            # commonディレクトリにあるcredentials.jsonを確認
            if not os.path.exists(COMMON_CREDS_PATH):
                raise FileNotFoundError(f"Missing {COMMON_CREDS_PATH}. Please place it there.")
            
            flow = InstalledAppFlow.from_client_secrets_file(COMMON_CREDS_PATH, SCOPES)
            creds = flow.run_local_server(port=0)
        
        # 認証結果を共通の場所へ保存
        with open(COMMON_TOKEN_PATH, 'w') as token:
            token.write(creds.to_json())

    return build('sheets', 'v4', credentials=creds)