from flask import Flask, render_template, request, jsonify, send_file
from flask_cors import CORS
import subprocess
import os
import tempfile
from io import BytesIO

app = Flask(__name__)
CORS(app)

def binary_string_to_bytes(binary_string): # for compress
    # Ensure the length of the binary string is a multiple of 8
    while len(binary_string) % 8 != 0:
        binary_string = "0" + binary_string

    # Convert binary string to bytes
    byte_array = bytearray()
    for i in range(0, len(binary_string), 8):
        byte_value = int(binary_string[i:i+8], 2)
        byte_array.append(byte_value)

    return bytes(byte_array)

def bytes_to_binary_string(byte_data): # for decompress
    binary_string = ''.join(format(byte, '08b') for byte in byte_data)
    return binary_string

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/compress', methods=['POST'])
def compress():
    try:
        # Handle file upload and save it to a temporary file
        file = request.files['file']
        temp_file = tempfile.NamedTemporaryFile(delete=False)
        temp_file.write(file.read())
        temp_file_path = temp_file.name
        temp_file.close()

        # Call C backend with the temporary file path
        result = subprocess.run(['./backend/huffman', temp_file_path], stdout=subprocess.PIPE, check=True)

        # Change binary string to binary data
        binary_string = result.stdout.decode("utf-8")
        binary_object = BytesIO(binary_string_to_bytes(binary_string))

        # Clean up the temporary file
        os.remove(temp_file_path)

        # Send the binary file in the response with the correct content type
        return send_file(
            binary_object,
            as_attachment=True,
            download_name='output.bin',
            mimetype='application/octet-stream'
        )
    except Exception as e:
        print(f"Error: {e}")
        return jsonify({'status': 'error', 'message': str(e)}), 500

if __name__ == '__main__':
    app.run(debug=True)
