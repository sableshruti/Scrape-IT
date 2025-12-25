import json
import re
import os
import requests
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
from selenium.common.exceptions import StaleElementReferenceException, NoSuchElementException
from bs4 import BeautifulSoup
from fpdf import FPDF

def download_pdf(url, save_path):
    # Send a GET request to the webpage
    response = requests.get(url)
    with open(save_path, 'wb') as file:
        file.write(response.content)
    print(f"Downloaded PDF from {url} to {save_path}")

def remove(string):
    pattern = re.compile(r"\s+")
    return re.sub(pattern, "", string)

def write_to_cpp_file(path, file_name, data):
    file_path_name_w_ext = os.path.join(path, file_name)
    with open(file_path_name_w_ext, "w", encoding="utf-8") as file:
        file.write(data)

# Initialize WebDriver
driver = webdriver.Chrome()
driver.get("https://www.cse.iitb.ac.in/~cs101/lectures.html")
wait = WebDriverWait(driver, 10)

for j in range(2, 30):
    try:
        # Refetch the name element to avoid stale reference
        name_xpath = f"/html/body/div[2]/div[2]/div/div/table/tbody/tr[{j}]/td[3]/a[1]"
        name_element = wait.until(EC.presence_of_element_located((By.XPATH, name_xpath)))
        name_text = name_element.text.strip()
        os.makedirs(f"./files/{name_text}", exist_ok=True)

        path1 = f"/html/body/div[2]/div[2]/div/div/table/tbody/tr[{j}"
        path3 = "]/td[4]/a["
        for k in range(1, 15):
            path = f"{path1}{path3}{k}]"
            driver.switch_to.default_content()
            try:
                course_element = wait.until(EC.presence_of_element_located((By.XPATH, path)))
                course_code = remove(course_element.text)
                course_element.click()
                
                try:
                    course_name_element = wait.until(EC.presence_of_element_located((By.XPATH, "/html/body/pre")))
                    data = course_name_element.text
                    write_to_cpp_file(f"./files/{name_text}", course_code, data.replace("\n", '\n'))
                except NoSuchElementException:
                    continue

                driver.back()
            except (NoSuchElementException, StaleElementReferenceException):
                break

        # Handle PDF download
        pdf_xpath = f"/html/body/div[2]/div[2]/div/div/table/tbody/tr[{j}]/td[3]/a[2]"
        try:
            pdf_element = wait.until(EC.presence_of_element_located((By.XPATH, pdf_xpath)))
            pdf_url = pdf_element.get_attribute('href')
            pdf_save_path = f"./files/{name_text}/slides.pdf"
            pdff = FPDF()
            pdff.output(f"./files/{name_text}/slides.pdf")
            download_pdf(pdf_url, pdf_save_path)
        except NoSuchElementException:
            pass

    except Exception as e:
        print(f"Error for row {j}: {e}")

driver.quit()
print("Done")
