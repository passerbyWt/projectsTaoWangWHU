from flask import Flask,session
from ChatBot import *
import os
from datetime import timedelta

app = Flask(__name__)
app.config['SECRET_KEY']=os.urandom(24)
app.config['PERMANENT_SESSION_LIFETIME']=timedelta(days=1)

@app.route('/')
def welcome():
    return 'welcome!'



@app.route('/chat', methods=['POST','GET'])
def chat():

    message=None

    reply=ChatBot().chat(message=None)


    return reply





if __name__ == '__main__':
    app.run()
