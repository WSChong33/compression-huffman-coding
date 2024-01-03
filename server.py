from flask import Flask, render_template, request, jsonify
from flask_cors import CORS
import subprocess
import os
import tempfile

app = Flask(__name__)
CORS(app)

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/compress', methods=['POST'])
def compress():
    try:
        print("test")

        # Handle file upload and save it to a temporary file
        file = request.files['file']
        temp_file = tempfile.NamedTemporaryFile(delete=False)
        temp_file.write(file.read())
        temp_file_path = temp_file.name
        temp_file.close()

        # Call C backend with the temporary file path
        subprocess.run(['./backend/huffman', temp_file_path], check=True)

        # Clean up the temporary file
        os.remove(temp_file_path)

        # Send response
        return jsonify({'status': 'success'})
    except Exception as e:
        print(f"Error: {e}")
        return jsonify({'status': 'error', 'message': str(e)}), 500

if __name__ == '__main__':
    app.run(debug=True)
