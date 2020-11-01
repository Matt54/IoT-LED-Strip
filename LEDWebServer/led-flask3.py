#LED-control

from flask import Flask, request, render_template, redirect, url_for
import smbus
import time

bus = smbus.SMBus(1)
address=0x08

app = Flask(__name__) #create the Flask app

@app.route('/')
def main_page():
    data = get_data()
    return render_template('main.html', data=data)

@app.route('/SettingChange', methods=['GET', 'POST'])
def formexample():
    if request.method == 'POST':
        timing = request.form.get('timing')
        mod = request.form.get('mod')
        program_number = request.form.get('program_number')
        brightness = request.form.get('brightness')
        audioReactive = 2
        if request.form.get('arCheck'):
            audioReactive = 1
        else:
            audioReactive = 0

        isOn = 2
        if request.form.get('powerCheck'):
            isOn = 1
        else:
            isOn = 0

        if(timing != ""):
            write(0,int(timing))
        if(mod != ""):
            write(1,int(mod))
        if(program_number != ""):
            write(2,int(program_number))
        if(brightness != ""):
            write(3,int(brightness))
        if(audioReactive != 2):
            write(4,int(audioReactive))
        if(isOn != 2):
            write(6,int(isOn))

    return redirect(url_for('main_page'))
    #data = get_data()
    #return render_template('main.html', data=data)


@app.route('/TogglePower', methods=['GET', 'POST'])
def togglePower():
    write(7,int(1))
    return redirect(url_for('main_page'))

def write(register,value):
    bus.write_byte_data(address,register,value)
    wait()
    return -1

def read(register):
    write(5,5)
    return bus.read_byte_data(address,0x00)

def get_data():
    write(5,0)
    timing = str(read_data())
    write(5,1)
    mod = str(read_data())
    write(5,2)
    program_number = str(read_data())
    write(5,3)
    brightness = str(read_data())
    write(5,4)
    audioReactive = str(read_data())
    write(5,5)
    isOn = str(read_data())

    #message = get_data()

    data = []
    data.append(timing)
    data.append(mod)
    data.append(program_number)
    data.append(brightness)
    data.append(audioReactive)
    data.append(isOn)
    return data

def read_data():
    return bus.read_byte(address);

def wait(timeToWait=0.15):
    start = time.time()
    while time.time() - start < timeToWait:
        pass

if __name__ == "__main__":
    app.run(host='0.0.0.0',port=2525,debug=True) #run app in debug mode on port 2525
