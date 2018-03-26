const SerialPort = require("serialport");
const Readline = SerialPort.parsers.Readline;
const port = new SerialPort("/dev/cu.wchusbserial5d10", {
  baudRate: 9600
});

let ready = false;
let error = false;
let awaitingAck = [];
let awaitingData = [];

port.on("error", (err) => {
	ready = false;
	error = true;
	console.log("Error: ", err.message);
});

const parser = new Readline();
port.pipe(parser);
parser.on('data', (d) => {
	const data = d.toString().trim();
	if (awaitingAck.length && data === "A") {
		awaitingAck.shift()();
	} else if(awaitingData.length && data !== "A") {
		awaitingData.shift()(data);
	} else {
		console.log("Received unexpected data:", data.toString());
	}
});

async function on() {
	const done = new Promise((resolve, reject) => {
		awaitingAck.push(resolve);
	});
	port.write("I");
	await done;
}

async function off() {
	const done = new Promise((resolve, reject) => {
		awaitingAck.push(resolve);
	});
	port.write("O");
	await done;
}

async function flash() {
	const done = new Promise((resolve, reject) => {
		awaitingAck.push(resolve);
	});
	port.write("F");
	await done;
}

async function temp() {
	const dataReceived = Promise.all([
		new Promise((resolve, reject) => {
			awaitingData.push(resolve);
		}),
		new Promise((resolve, reject) => {
			awaitingData.push(resolve);
		}),
		new Promise(function (resolve, reject) {
			awaitingAck.push(resolve);
		})
	]);
	port.write("T");
	const data = await dataReceived;
	return {
		temperature: data[0],
		humidity: data[1]
	};
}

async function checkReady() {
	if (!ready) {
		await Promise.all([
			// new Promise(function (resolve, reject) {
			// 	awaitingAck.push(() => {
			// 		console.log("Received initial ACK");
			// 		resolve();
			// 	});
			// }),
			new Promise(function (resolve, reject) {
				port.on("open", function() {
					console.log("Serial connection ready");
					resolve();
				});
			})
		]);
		ready = true;
	}

}
checkReady().then(() => {
	var stdin = process.openStdin();

	stdin.addListener("data", function(d) {
		const com = d.toString().trim();
	    switch(com) {
			case "I":
				on().then(() => console.log("Done"));
				break;
			case "O":
				off().then(() => console.log("Done"));
				break;
			case "F":
				flash().then(() => console.log("Done"));
				break;
			case "T":
				temp().then((data) => console.log(data));
				break;
	    }
  	});
});