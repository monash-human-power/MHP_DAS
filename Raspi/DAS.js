/* Set up modules */
// Set up server connection
const request = require('request-promise-native');
const DAS_SERVER_ADDR = "http://127.0.0.1:5000";
var IS_SERVER_CONNECTED = false;


// Set up serial port connection
var SerialPort = require('serialport');
var serialport_options = {
	autoOpen: false,
	baudRate: 500000,
	dataBits: 8,
	stopBits: 1,
	parity: 'none'
}
var serialPort = new SerialPort('/dev/serial0', serialport_options, (err) => {
	// Print out error with opening serial port
	if (err) {
		console.log(err);
	}
});
// Parse incoming data;
var Readline = SerialPort.parsers.Readline;
var parser = new Readline();
serialPort.pipe(parser);


/* Start of main code */
// Check if server is connected
function check_if_server_online() {
	// Sends a GET request to the /server/status endpoint
	request.get(DAS_SERVER_ADDR + '/server/status', {timeout: 1000}, (error, res, body) => {
		if (error) {
			console.log(error);
			return false;
		}

		if (res.statusCode == 200) {
			// Stop asking server if online
			clearInterval(find_server_interval);
			// Run main code after function returns
			setImmediate(main);
			return true;
		} else {
			return false;
		}
	});
}

// Check if server is online every 1.5 seconds
const find_server_interval = setInterval(() => {
	console.log("Finding server");
	check_if_server_online()
}, 1500);

function main(){
	// Open event to tell us when connection with teensy has been made
	serialPort.open((err) => {
		if (err) {
			return console.log(err.message);
		}
		console.log('Serial port open');
	})
}

function create_file_name() {
	return request.get(DAS_SERVER_ADDR + '/files', (error, response, body) => {
		if (error) {
			console.log(error);
			return false;
		}
	})
}

serialPort.on("open", () => {
	console.log('Port opened with Teensy');
	
	// Server and Teensy need to be both connected to receive data
	parser.on('data', (data) => {
		console.log(data);
		if (data == 'start') {
			create_file_name()
				.then( body => {
					console.log(body);
				})
		}
	});
})
