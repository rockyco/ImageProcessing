from PIL import Image

def process_image(image_path, output_path, resolution=(64, 64)):
    """
    Reads a PNG image, converts it to grayscale with adjustable resolution,
    and prints the pixel data to a TXT file as stimulus input.

    Args:
        image_path (str): Path to the input PNG image.
        output_path (str): Path to the output TXT file.
        resolution (tuple): Desired resolution (width, height) of the output image.
    """
    try:
        # Open the image
        img = Image.open(image_path)

        # Resize the image
        img = img.resize(resolution)

        # Convert to grayscale
        img = img.convert('L')

        # Get pixel data
        pixel_data = list(img.getdata())

        # Write pixel data to TXT file
        with open(output_path, 'w') as f:
            for pixel_value in pixel_data:
                f.write(str(pixel_value) + '\n')

        print(f"Image processed and saved to {output_path}")

    except FileNotFoundError:
        print(f"Error: Image file not found at {image_path}")
    except Exception as e:
        print(f"An error occurred: {e}")

if __name__ == "__main__":
    # Example usage:
    image_path = 'test.png'  # Replace with your image path
    output_path = 'pic_in.txt'  # Replace with your desired output path
    resolution = (640, 480)  # Replace with your desired resolution

    process_image(image_path, output_path, resolution)
