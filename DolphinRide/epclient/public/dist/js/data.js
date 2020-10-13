(function() {

    /*
     * Class for generating real-time data for the area, line, and bar plots.
     */
    var RealTimeData = function(layers) {
        this.layers = layers;
        this.timestamp = ((new Date()).getTime() / 1000)|0;
        this.start_timestamp = 0;
    };

    RealTimeData.prototype.history = function(entries) {
        var history = [];
        for (var k = 0; k < this.layers; k++) {
            history.push({ values: [] });
        }

        this.start_timestamp = this.timestamp;

        var t = this.start_timestamp;

        for (var i = 0; i < entries; i++) {
            for (var j = 0; j < this.layers; j++) {
                history[j].values.push({time: t, y: 0});
            }
            t++;
        }

        return history;
    };

    RealTimeData.prototype.next = function(v) {
        var entry = [];
        for (var i = 0; i < this.layers; i++) {
            entry.push({ time: this.timestamp, y: v});
        }
        return entry;
    }

    RealTimeData.prototype.nextFrame = function() {
        this.timestamp++;
        return ;
    }

    window.RealTimeData = RealTimeData;

    /*
     * Gauge Data Generator.
     */
    var GaugeData = function() {};

    window.GaugeData = GaugeData;

})();
