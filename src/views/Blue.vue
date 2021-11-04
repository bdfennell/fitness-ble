<template>
  <v-container>
    <div>
      {{core}}
    </div>
    <v-btn @click="pair()">Pair</v-btn>
    <div>
      {{server}}
    </div>
  </v-container>
</template>
<script>
// import noble from 'noble'
export default {
  name: 'Blue',
  data() {
    return {
      core: true,
      uuid: [],
      bike: {
        id: "LHCaZ78RZ9sQdKygg6OiqQ==",
        name: "ECHEX-3-145257"
      },
      server: null
    }
  },
  methods: {
    pair() {
      // navigator.bluetooth.requestDevice({acceptAllDevices:true}).then(result => {
      // console.log(result)

      navigator.bluetooth.requestDevice({
        acceptAllDevices:true
        // filters: [{
        //   name: 'ECHEX-3-145257',
        //   optionalServices: ['0bf669f0-45f2-11e7-9598-0800200c9a66',0x69f0,'0bf669f1-45f2-11e7-9598-0800200c9a66',0x69f1]
        // }]
      }).then(device => {
        return device.gatt.connect()
      }).then(server => {
        console.log(server.getPrimaryService(0x69f1))
      }).then(services => {
        console.log(services)
      })




    }
  }
}
</script>
