from flask import Flask, request, make_response
import time
import json
import base64


app = Flask("test server")

# 语音聊天
@app.route("/voice", methods=["POST"])
def voice():
    
    fileName = "recvFiles\\"
    fileName += "clientVoice"  
    fileName += ".wav"

    # 客户端发来的语音数据是json格式
    # {
    #    "Voice": "base64-encoded string of .wav"
    # }
    # 先解析json，再取出Voice字段进行base64解码，得到.wav数据(16000采样率，单声道)
    voice = json.loads(request.data.decode())["Voice"]

    wavData = base64.b64decode(voice.encode())

    # text = process(wavData) # 语音识别的结果: text

    # newText = GPT(text) # 对话文本生成 

    # newWavData = tts(newText) # 文本生成语音

    res = dict()

    # res["Text"] = newText
    res["Text"] = "这是语音识别+文本处理后的结果"

    # 测试音频
    data = open("serverFiles/nen111_061.wav", "rb").read()

    # res[Sound] = base64.b64encode(newWavData).decode()
    res["Sound"] = base64.b64encode(data).decode()

    return res


# 文本聊天
@app.route("/chat", methods=["GET"])
def chat():
    # 接收客户端的聊天文本
    text = request.args.get("Text", "")

    print("文本: %s" % text)

    # 构造响应
    rsp = make_response()

    # 响应头中添加Text字段
    rsp.headers.add_header("Text", "坐在电脑前很久了哦，快去休息一下吧！".encode("utf-8"))

    # 响应body中写入音频数据
    with open("serverFiles\\nen111_061.wav", "rb") as f:
        rsp.set_data(f.read())

    return rsp

if __name__ == "__main__":

    app.run(host="127.0.0.1", port=50721)