from flask import Flask, request, jsonify
from flask_cors import CORS
import os
import subprocess

app = Flask(__name__)
CORS(app)


@app.route('/run', methods=['POST'])
def run(): #存储运算表达式和值
    try:
        data = request.get_json()  # 获取POST请求的JSON数据
        sampleIn = data.get('inputText')
        print(sampleIn)
        with open('sample.txt', 'w') as sample_in_file:
            sample_in_file.write(sampleIn)

        subprocess.run(['run_programs.exe'], capture_output=True, text=True)
        with open('answer.txt', 'r', encoding='utf-8') as output_file:  # 确保指定正确的编码
            output = output_file.read()
        return jsonify({"message": output})

    except Exception as e:
        error_message = str(e)
        return jsonify({"error": error_message}), 500


if __name__ == '__main__':
    app.run(debug=True)
