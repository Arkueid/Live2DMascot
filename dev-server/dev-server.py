# python implementation of chat server
from flask import Flask, request, make_response
import random
import base64
import json
app = Flask(__name__)


def nlg(text: str) -> str:
    print(text) # to test if input is valid

    # do something here

    return "response text"


def asr(soundBytes: bytes) -> str:
    if (soundBytes):
        with open('temp-voice-input.wav', 'wb') as f:
            f.write(soundBytes) # to test if input is valid

    # do something here

    return "result of asr"


def tts(text: str) -> str:

    # do something here

    return base64.b64encode(open('audio_%d.wav' % (random.randint(1,3)), 'rb').read()).decode() # bytes str


@app.route("/text", methods=['POST']) # post only
def text_chat():
    req = request.get_json()
    rsp = make_response()
    d = dict()
    d['Text'] = nlg(req['Text'])
    d['Sound'] = tts(d['Text'])
    rsp.data = json.dumps(d)
    return rsp


@app.route("/voice", methods=['POST']) # post only
def voice_chat():
    req = request.get_json()
    rsp = make_response()
    d = dict()
    d['ASR'] = asr(base64.b64decode(req["Voice"].encode()))
    d['Text'] = nlg(d['ASR'])
    d['Sound'] = tts(d['Text'])
    rsp.data = json.dumps(d)
    return rsp


if __name__ == "__main__":
    app.run("127.0.0.1", 5000)