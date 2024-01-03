const compress = () => {
    const fileInput = document.getElementById('fileInput');
    console.log('File Input Element:', fileInput);

    if (!fileInput || !fileInput.files || fileInput.files.length === 0) {
        console.error('No file selected.');
        return;
    }

    const file = fileInput.files[0];
    console.log('Selected File:', file);

    const formData = new FormData();
    formData.append('file', file);

    console.log('FormData:', formData);

    fetch('/compress', {
        method: 'POST',
        body: formData
    })
    .then(response => {
        console.log('Server Response:', response);
        return response.json();
    })
    .then(data => {
        console.log('Parsed JSON Data:', data);
        console.log('Status:', data.status);
        // Handle success or show a message to the user
    })
    .catch(error => {
        console.error('Error:', error);
        // Handle error or show a message to the user
    });
};
