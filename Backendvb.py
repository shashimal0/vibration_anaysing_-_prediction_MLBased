import gspread
import json
from google.oauth2.service_account import Credentials
from datetime import datetime
import pandas as pd
import pickle
import time

gc = gspread.service_account(filename='credentials.json')

with open('predictor2.pickle', 'rb') as model_file:
    model = pickle.load(model_file)

while True:
    try:
        #pred_val=model.predict([[-0.04,-0.02,-0.01,0,-0.86,0.42]])
        pred_val=model.predict([[0.2,0.02,-0.17,-0.37,-2.81,0.61]])
        #["-0.04", "-0.04", "0.01", "-0.37", "-0.74", "0.36", "-78.61", "0", "2.18"]
        #["-0.05", "-0.03", "-0.01", "-0.19", "-0.74", "0.36", "-78.61", "0", "2.18"]
        #["-0.04", "-0.02", "-0.01", "0", "-0.86", "0.42", "-78.61", "0", "2.18"]
        print(pred_val)
        if (pred_val==1):
            PredString="Good"
        elif (pred_val==0):
            PredString="Good"
        else:
            PredString="Old"

        time.sleep(1)
    except Exception as e:
        print("An error occurred:", str(e))
        print("______________________________________________________________________________")
