# chatgpt goes brr

from google.oauth2.credentials import Credentials
from google_auth_oauthlib.flow import InstalledAppFlow
from google.auth.transport.requests import Request
from googleapiclient.discovery import build

import socket
import datetime
import os.path
import pickle
import pytz

# If modifying these SCOPES, delete the file token.pickle.
SCOPES = ['https://www.googleapis.com/auth/calendar.readonly']
CREDENTIALS_FILE = 'credentials.json'
TOKEN_PICKLE_FILE = 'token.pickle'
SEVER_PORT = 16662
SERVER_ADDRESS = '10.166.0.221'

def send_str_to_server(string : str): 
    
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        try:
            s.connect((SERVER_ADDRESS, SEVER_PORT))
            s.sendall(string.encode('utf-8'))
        except Exception as e:
            print(f"Error: {e}")

def get_credentials():
    creds = None
    if os.path.exists(TOKEN_PICKLE_FILE):
        with open(TOKEN_PICKLE_FILE, 'rb') as token:
            creds = pickle.load(token)
    if not creds or not creds.valid:
        if creds and creds.expired and creds.refresh_token:
            creds.refresh(Request())
        else:
            flow = InstalledAppFlow.from_client_secrets_file(
                CREDENTIALS_FILE, SCOPES)
            creds = flow.run_local_server(port=0)
        with open(TOKEN_PICKLE_FILE, 'wb') as token:
            pickle.dump(creds, token)
    return creds
    
# return list of events as a string
def list_all_events():
    creds = get_credentials()
    service = build('calendar', 'v3', credentials=creds)
    out = ""

    # Call the Calendar API
    calendar_list = service.calendarList().list().execute()
    
    # Set the time zone to Eastern Time
    eastern = pytz.timezone('America/New_York')

    for calendar in calendar_list['items']:
        
        if (calendar['summary'] != "Personal Schedule"):
            continue
        
        #print(f"Events for calendar: {calendar['summary']}")
        calendar_id = calendar['id']
        
        now = datetime.datetime.utcnow()
        end_of_day = now + datetime.timedelta(days=1)  # Adjust the time range as needed

        events_result = service.events().list(
            calendarId=calendar_id,
            timeMin=now.isoformat() + 'Z',
            timeMax=end_of_day.isoformat() + 'Z',
            singleEvents=True,
            orderBy='startTime'
        ).execute()

        events = events_result.get('items', [])

        if not events:
            print('No upcoming events found.')
            
        for event in events:
            start_time = event['start'].get('dateTime', event['start'].get('date'))
            start_time_utc = datetime.datetime.fromisoformat(start_time.replace('Z', '+00:00'))
            start_time_eastern = start_time_utc.astimezone(eastern)

            date_str = start_time_eastern.strftime('%Y-%m-%d')
            time_str = start_time_eastern.strftime('%H:%M:%S')

            out += f"<ac>{date_str}\n<al>{time_str} - {event['summary']}\n"
        
        # only print the first personal schedule
        return out 

if __name__ == '__main__':
    message = "<ac>My Schedule:\n"
    message += list_all_events()
    send_str_to_server(f"PRINT\n\n{message}\n\n")