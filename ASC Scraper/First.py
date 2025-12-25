import json
import re
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.common.keys import Keys

USERNAME = "24b0986"  # Ldap ID
PASSWORD = "*********"  # Ldap Password

def remove(string):
    pattern = re.compile(r"\\s+")
    return re.sub(pattern, "", string)

driver = webdriver.Chrome()  # Ensure ChromeDriver is installed
driver.get("https://asc.iitb.ac.in/acadmenu/index.jsp") 

driver.switch_to.frame("rightPage")
user_name = driver.find_element(By.NAME, "UserName")
pw = driver.find_element(By.NAME, "UserPassword")
user_name.send_keys(USERNAME)
pw.send_keys(PASSWORD)
pw.send_keys(Keys.RETURN)
driver.implicitly_wait(1)

driver.switch_to.frame(0)
acad = driver.find_element(
    By.XPATH, "/html/body/div[1]/table/tbody/tr/td[2]/div/div/div/div/div[1]/table/tbody/tr/td[2]/a"
)
acad.click()
abc = driver.find_element(By.PARTIAL_LINK_TEXT, "All About")
abc.click()
run = driver.find_element(By.PARTIAL_LINK_TEXT, "Running Courses")
run.click()
driver.implicitly_wait(5)
run = driver.find_element(By.PARTIAL_LINK_TEXT, "Running Courses")

driver.switch_to.default_content()
driver.switch_to.frame("rightPage")

year = driver.find_element(
    By.XPATH, "/html/body/table/tbody/tr[1]/td/form/select[1]/option[15]"
)  # 2024-25
year.click()
sem = driver.find_element(
    By.XPATH, "/html/body/table/tbody/tr[1]/td/form/select[2]/option[1]"
)  # Autumn
sem.click()
go = driver.find_element(By.XPATH, "/html/body/table/tbody/tr[1]/td/form/input")
go.click()


driver.switch_to.default_content()
driver.switch_to.frame("rightPage")
dept = driver.find_element(
    By.XPATH, "/html/body/table/tbody/tr[" + str(10) + "]/td[1]"
)
print(dept.text)  # To get department name **Remove Later**
driver.implicitly_wait(2)
dep = driver.find_element(By.PARTIAL_LINK_TEXT, dept.text)
driver.implicitly_wait(2)
dep.click()
driver.implicitly_wait(2)

all_details = []
for j in range(5, 10):
        path1 = "/html/body/table/tbody/tr[" + str(j)
        path3 = "]/td[3]"
        path = path1 + path3 + "/a"  # for course_code field

        driver.switch_to.default_content()
        driver.switch_to.frame("rightPage")

        driver.implicitly_wait(2)
        try:
            corse = driver.find_element(By.XPATH, path)
        except:
            break
        course_code = corse.text
        course_code = remove(course_code)
        corse.click()
        driver.implicitly_wait(5)
        # # Your code here
        try:
            course_name = driver.find_element(By.XPATH, "/html/body/form/table/tbody/tr[2]/td[2]")
        except:
            continue
        total_credits = driver.find_element(
            By.XPATH, "/html/body/form/table/tbody/tr[3]/td[2]")
        course_type = driver.find_element(
            By.XPATH, "/html/body/form/table/tbody/tr[4]/td[2]")
        course_lecture = driver.find_element(
            By.XPATH, "/html/body/form/table/tbody/tr[5]/td[2]")
        course_tutorial = driver.find_element(
            By.XPATH, "/html/body/form/table/tbody/tr[6]/td[2]")
        course_practical = driver.find_element(
            By.XPATH, "/html/body/form/table/tbody/tr[7]/td[2]")
        is_half_sem = driver.find_element(
            By.XPATH, "/html/body/form/table/tbody/tr[9]/td[2]")
        text_ref = driver.find_element(
            By.XPATH, "/html/body/form/table/tbody/tr[10]/td[2]")
        description = driver.find_element(
            By.XPATH, "/html/body/form/table/tbody/tr[11]/td[2]")
        # # get all the fields

        all_details.append({
            'code': course_code,
            'name': course_name.text,
            'credits': total_credits.text,
            'type': course_type.text,
            'lecture': course_lecture.text,
            'tutorial': course_tutorial.text,
            'practical': course_practical.text,
            'is_half_sem': is_half_sem.text,
            'text_ref': text_ref.text,
            'desc': description.text
        })
        driver.back()
        # print(all_details)
        # break

driver.back()

def writeToJSONFile(path, fileName, data):
    filePathNameWExt = "./" + path + "/" + fileName + ".json"
    with open(filePathNameWExt, "w") as fp:
        json.dump(data, fp, indent = 4)

path = "./"
fileName = "courses"

writeToJSONFile("./", fileName, all_details)

print("Done")
