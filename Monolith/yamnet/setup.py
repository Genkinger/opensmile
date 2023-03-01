from requests import get
from subprocess import call

# This script will set up everything necessary for yamnet

yamnet_h5_model_url = "https://storage.googleapis.com/audioset/yamnet.h5"
tensorflow_models_git_url = "https://github.com/tensorflow/models.git"

# Clone the repository into ./models
print("Starting git clone...")
call(["git", "clone", tensorflow_models_git_url])

# Download the pre-trained model and save it to the right location
print("Starting download of 'yamnet.h5'...")
response = get(yamnet_h5_model_url, allow_redirects=True)
with open("./models/research/audioset/yamnet/yamnet.h5", "wb+") as model_file:
    model_file.write(response.content)

print("All done!")
# Done
