import requests
import os

def download_image(image_url, save_directory, image_name):
    if not os.path.exists(save_directory):
        os.makedirs(save_directory)
        
    response = requests.get(image_url)
    if response.status_code == 200:
        with open(os.path.join(save_directory, image_name), 'wb') as file:
            file.write(response.content)
        print(f"Image saved as {image_name} in {save_directory}")
    else:
        print("Failed to download image. Status code:", response.status_code)

base_url = "https://image.slidesharecdn.com/role-of-digital-marketing-igniting-business-growth-in-the-digital-age-240209071145-aa6a8625/75/importance-of-digital-marketing-ppt-"
save_directory = "downloaded_images"

for i in range(1, 15):
    image_url = f"{base_url}{i}-2048.jpg"
    image_name = f"downloaded_image_{i}.jpg"
    download_image(image_url, save_directory, image_name)