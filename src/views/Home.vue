<template>
  <v-container dark>
    <div>
      <v-switch v-model="recordData"></v-switch>
    </div>


    <v-card dark class="ma-5 pa-5 text-center">
      <div class="overline">Cadence</div>
      <div class="display-4">
        {{lastCadence}}

      </div>
      <v-sparkline
        :value="cadence"
        :line-width="5"
        auto-draw
        smooth
        color="green"
      ></v-sparkline>
    </v-card>
    <v-card dark class="ma-5 pa-5 text-center ">
      <div class="overline">Resistance</div>
      <div class="display-4">
        {{lastResistance}}

      </div>
      <v-sparkline
        :value="resistance"
        :line-width="5"
        auto-draw
        smooth
        color="blue"
      ></v-sparkline>
    </v-card>



  </v-container>



</template>

<script>
import ParticleAuth from '../ParticleAuth.js'
var Particle = require('particle-api-js');
var particle = new Particle();



export default {
  name: 'Home',



  data() {
    return {
      cadence: [],
      resistance: [],
      lastCadence: 0,
      lastResistance: 0,
      recordData: false
    }
  },
  computed: {

  },
  mounted() {

    particle.getEventStream({ name: 'cadence', auth: ParticleAuth.KEY})
    .then((stream)=>{
      stream.on('event', (data)=>{
        console.log(data)
        this.lastCadence  = parseInt(data.data)
        this.recordData ? this.record() : false

      });
    });
    particle.getEventStream({ name: 'resistance', auth: ParticleAuth.KEY})
    .then((stream)=>{
      stream.on('event', (data)=>{
        console.log(data)
        this.lastResistance = parseInt(data.data)
        this.recordData ? this.record() : false
      });
    });

  },
  methods: {
    record() {
      this.cadence.push(this.lastCadence)
      this.resistance.push(this.lastResistance)
    }
  }
}
</script>
