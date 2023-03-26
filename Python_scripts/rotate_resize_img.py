
import sys
import os
from PIL import Image

DEFAULT_SOURCE_DIR = "images"
DEFAULT_TARGET_DIR = "icons"

def process_image(img_file):
    im = img_file.rotate(90).resize((128,128))
    return im



def process_files(img_dir, target_dir):
    for dir_path,dir_name,file_names in os.walk(img_dir):
        for file in file_names:
            input_file = os.path.join(dir_path,file)
            output_file = os.path.join(target_dir,os.path.splitext(os.path.basename(input_file))[0]) + ".jpeg"
            try:
                with Image.open(input_file) as im:
                    print("Processing file: " + input_file)
                    if im.mode != 'RGB':
                        im = im.convert('RGB')
                    img = process_image(im)
            except OSError:
                print("Cannot process file: " + input_file)
                continue
            img.save(output_file)
            print("Processed file: " + output_file)

def main(arg):
    if len(arg) > 3:
        raise BaseException("Too much arguments")

    elif len(arg) == 3:
        work_dir = arg[1]
        if not os.path.exists(work_dir):
            raise ValueError("No such source directory")
        target_dir = arg[2]
    elif len(arg) == 2:
        work_dir = arg[1]
        if not os.path.exists(work_dir):
            raise ValueError("No such source directory")
        target_dir = os.path.join(os.path.dirname(__file__), DEFAULT_TARGET_DIR)
    else:
        work_dir = os.path.join(os.path.dirname(__file__), DEFAULT_SOURCE_DIR)
        if not os.path.exists(work_dir):
            raise ValueError("Default directory \"images\" not found. Create directory or set another path with source files")
        target_dir = os.path.join(os.path.dirname(__file__), DEFAULT_TARGET_DIR)
    try:
        os.mkdir(target_dir)
    except:
        pass
    process_files(work_dir,target_dir)

if __name__ == "__main__":
    main(sys.argv)


