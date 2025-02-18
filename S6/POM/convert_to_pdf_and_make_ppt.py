import os
from PIL import Image
from PyPDF2 import PdfMerger

def convert_images_to_pdfs(image_folder, pdf_folder):
    if not os.path.exists(pdf_folder):
        os.makedirs(pdf_folder)
    
    for i in range(1, 15):
        image_path = os.path.join(image_folder, f"downloaded_image_{i}.jpg")
        pdf_path = os.path.join(pdf_folder, f"converted_{i}.pdf")

        with Image.open(image_path) as img:
            img.convert("RGB").save(pdf_path)
        
        print(f"Image {image_path} converted to PDF {pdf_path}")

def merge_pdfs(pdf_folder, output_pdf):
    pdf_merger = PdfMerger()
    for i in range(1, 15):
        pdf_path = os.path.join(pdf_folder, f"converted_{i}.pdf")
        pdf_merger.append(pdf_path)
    
    pdf_merger.write(output_pdf)
    pdf_merger.close()
    print(f"All PDFs merged into {output_pdf}")

image_folder = "downloaded_images"
pdf_folder = "converted_pdfs"
output_pdf = "digital_marketing_for_modern_organizations.pdf"

convert_images_to_pdfs(image_folder, pdf_folder)
merge_pdfs(pdf_folder, output_pdf)