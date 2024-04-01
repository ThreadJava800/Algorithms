import random
import string
import subprocess

FILE_NAME = "input.txt"
mask_str = ""
text_str = ""

def generateTest():
    global mask_str
    global text_str

    mask_len = random.randrange(1e6)
    text_len = mask_len * random.randrange(1, 50)

    mask_str = "".join(random.choices(string.ascii_lowercase + '?', k=mask_len))
    text_str = "".join(random.choices(string.ascii_lowercase,       k=text_len))

    file_to_write = open(FILE_NAME, "w")
    file_to_write.write(mask_str)
    file_to_write.write(text_str)
    file_to_write.close()

while True:
    generateTest()
    task = subprocess.Popen(('./lang'), stdin=open('input.txt'))
    task.wait()
    try:
        subprocess.check_output(...)
        print("OK")
    except subprocess.CalledProcessError as e:
        print(e.output)
        break