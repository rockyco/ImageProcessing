from PIL import Image

def process_image(input_path, output_path, resolution=(64, 64)):
    """
    Reads pixel data from a TXT file, reshapes it according to the specified resolution,
    and saves it as a grayscale PNG image.

    Args:
        input_path (str): Path to the input TXT file containing pixel data.
        output_path (str): Path to the output PNG image file.
        resolution (tuple): Resolution (width, height) of the output image.
    """
    try:
        # Read pixel data from TXT file
        with open(input_path, 'r') as f:
            pixel_data = [int(line.strip()) for line in f]

        # Check if the pixel data matches the expected size
        if len(pixel_data) != resolution[0] * resolution[1]:
            raise ValueError("Pixel data size does not match the specified resolution.")

        # Create a new grayscale image
        img = Image.new('L', resolution)

        # Reshape the pixel data and put it into the image
        img.putdata(pixel_data)

        # Save the image as PNG
        img.save(output_path, 'PNG')

        print(f"Image saved to {output_path}")

    except FileNotFoundError:
        print(f"Error: Input file not found at {input_path}")
    except ValueError as ve:
        print(f"Error: {ve}")
    except Exception as e:
        print(f"An error occurred: {e}")

if __name__ == "__main__":
    # Example usage:
    input_path = 'pic_out.txt'  # Path to the text file containing pixel data
    output_path = 'output.png'  # Desired output path for the PNG image
    resolution = (640, 480)  # Resolution of the image

    process_image(input_path, output_path, resolution)
