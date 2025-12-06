import os
import shutil
import PyPDF2

def contains_esophageal_varices(pdf_path):
    with open(pdf_path, 'rb') as file:
        reader = PyPDF2.PdfReader(file)
        text = ""
        for page in reader.pages:
            text += page.extract_text()
        if 'esophageal varices' in text.lower() and 'no varices' not in text.lower():
            return True
    return False

def move_folders(root_dir, esophageal_varices_dir, no_esophageal_varices_dir):
    if not os.path.exists(esophageal_varices_dir):
        os.makedirs(esophageal_varices_dir)
    if not os.path.exists(no_esophageal_varices_dir):
        os.makedirs(no_esophageal_varices_dir)
    
    for root, dirs, files in os.walk(root_dir):
        for dir_name in dirs:
            subfolder_path = os.path.join(root, dir_name)
            pdf_files = [f for f in os.listdir(subfolder_path) if f.endswith('.pdf')]
            image_files = [f for f in os.listdir(subfolder_path) if f.endswith(('.jpg', '.jpeg', '.png', '.bmp'))]
            
            if pdf_files and image_files:
                esophageal_varices_found = False
                for pdf_file in pdf_files:
                    pdf_path = os.path.join(subfolder_path, pdf_file)
                    if contains_esophageal_varices(pdf_path):
                        esophageal_varices_found = True
                        break
                
                root_folder = os.path.dirname(subfolder_path)
                if esophageal_varices_found:
                    dest_folder = os.path.join(esophageal_varices_dir, os.path.basename(root_folder))
                    if not os.path.exists(dest_folder): 
                        shutil.move(root_folder, dest_folder)
                        print(f"Moved: {root_folder} to {dest_folder} (contains 'esophageal varices')")
                    else:
                        print(f"Skipped: {root_folder} (already exists in 'Esophageal_Varices')")
                else:
                    dest_folder = os.path.join(no_esophageal_varices_dir, os.path.basename(root_folder))
                    if not os.path.exists(dest_folder): 
                        shutil.move(root_folder, dest_folder)
                        print(f"Moved: {root_folder} to {dest_folder} (does not contain 'esophageal varices')")
                    else:
                        print(f"Skipped: {root_folder} (already exists in 'No_Esophageal_Varices')")
                break  

root_directory = 'E:/gmc_data/all'
esophageal_varices_directory = 'E:/gmc_data/varices'
no_esophageal_varices_directory = 'E:/gmc_data/no_varices'

move_folders(root_directory, esophageal_varices_directory, no_esophageal_varices_directory)