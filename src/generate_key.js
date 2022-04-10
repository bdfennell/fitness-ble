var Particle = require('particle-api-js');
var particle = new Particle();
const process = require('process')


particle.login({username: process.env.USER, password: process.env.PASSWORD}).then(
  function(data) {
    token = data.body.access_token;
    console.log(token)
  },
  function (err) {
    console.log('Could not log in.', err);
  }
);


//USER=*** PASSWORD=*** node generate_key.js
