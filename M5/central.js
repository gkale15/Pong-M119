// code adapted from https://github.com/ucla-hci/m119/tree/main/m3
// What: Taking xmlHttp requests from the server, using accelerometer data from the Arduino
// Why: code was already there given as an example

const noble = require('@abandonware/noble');

const uuid_service = "1102"
const uuid_service_gyro = "1105"

const x_uuid = "2101"
const y_uuid = "2102"
const z_uuid = "2103"

const gx_uuid = "3101"
const gy_uuid = "3102"
const gz_uuid = "3103"

let  sensorValue = NaN
let  sensorValueX = NaN
let  sensorValueY = NaN
let  sensorValueZ = NaN
let  sensorValueGX = NaN
let  sensorValueGY = NaN
let  sensorValueGZ = NaN

noble.on('stateChange', async (state) => {
    if (state === 'poweredOn') {
        console.log("start scanning")
        await noble.startScanningAsync([uuid_service], false);
    }
});

noble.on('stateChange', async (state) => {
    if (state === 'poweredOn') {
        console.log("start scanning")
        await noble.startScanningAsync([uuid_service_gyro], false);
    }
});

noble.on('discover', async (peripheral) => {
    await noble.stopScanningAsync();
    await peripheral.connectAsync();
    const {
        characteristics
    } = await peripheral.discoverSomeServicesAndCharacteristicsAsync([uuid_service,uuid_service_gyro],[x_uuid,y_uuid,z_uuid,gx_uuid,gy_uuid,gz_uuid]);
    readData(characteristics[0])
    readData(characteristics[1])
    readData(characteristics[2])
    readData(characteristics[3])
    readData(characteristics[4])
    readData(characteristics[5])

});

//
// read data periodically
//
let readData = async (characteristic) => {
    const value = (await characteristic.readAsync());
    sensorValue = value.readFloatLE(0);
    // console.log(characteristic.uuid + ": " + sensorValue);
    if (characteristic.uuid == "2101") {
        sensorValueX = value.readFloatLE(0);
    }
    if (characteristic.uuid == "2102") {
        sensorValueY = value.readFloatLE(0);
    }
    if (characteristic.uuid == "2103") {
        sensorValueZ = value.readFloatLE(0);
    }
    if (characteristic.uuid == "3101") {
        sensorValueGX = value.readFloatLE(0);
    }
    if (characteristic.uuid == "3102") {
        sensorValueGY = value.readFloatLE(0);
    }
    if (characteristic.uuid == "3103") {
        console.log(characteristic.uuid + ": " + sensorValue);
        sensorValueGZ = value.readFloatLE(0);
    }

    // read data again in t milliseconds
    setTimeout(() => {
        readData(characteristic)
    }, 5);
}


const express = require('express')
const app = express()
const port = 3000

app.set('view engine', 'ejs');

app.get('/', (req, res) => {
    res.render('index')
})

app.post('/', (req, res) => {
    res.writeHead(200, {
        'Content-Type': 'application/json'
    });
    res.end(JSON.stringify({
        aX: sensorValueX,
        aY: sensorValueY,
        aZ: sensorValueZ,
        gX: sensorValueGX,
        gY: sensorValueGY,
        gZ: sensorValueGZ
    }))
})

app.listen(port, () => {
    console.log(`Example app listening on port ${port}`)
})