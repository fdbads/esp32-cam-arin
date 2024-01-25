import imageio

def images_to_video(image_folder, output_video_path, fps=30):
    image_files = sorted([img_file for img_file in os.listdir(image_folder) if img_file.endswith(('.png', '.jpg', '.jpeg'))])
    images = [imageio.imread(os.path.join(image_folder, img)) for img in image_files]

    imageio.mimsave(output_video_path, images, fps=fps)

# Example usage
image_folder_path = "path/to/your/image/folder"
output_video_path = "output_video.mp4"
images_to_video(image_folder_path, output_video_path)
