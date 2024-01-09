const updateFileName = () => {
    const input = document.getElementById('fileInput');
    const fileNameSpan = document.getElementById('fileName');
    fileNameSpan.textContent = input.files[0] ? input.files[0].name : 'Choose a file';
};


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

        // Check if the response is successful (status code 200)
        if (!response.ok) {
            throw new Error(`HTTP error! Status: ${response.status}`);
        }

        // Extract the binary data
        return response.blob();
    })
    .then(data => {
        console.log('Binary File Object:', data);

        // Create a blob URL for the binary data
        const blobUrl = URL.createObjectURL(data);

        // Create a link element
        const link = document.createElement('a');
        link.href = blobUrl;

        // Set the download attribute with the desired file name
        link.download = 'compressed.bin';

        // Append the link to the document
        document.body.appendChild(link);

        // Trigger a click event on the link to start the download
        link.click();

        // Remove the link from the document
        document.body.removeChild(link);
    })
    .catch(error => {
        console.error('Error:', error);
        // Handle error or show a message to the user
    });
};
