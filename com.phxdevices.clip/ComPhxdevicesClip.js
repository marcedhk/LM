/* ComPhxdevicesClip.js */

enyo.kind({
	name: "ComPhxdevicesClip",
	kind: enyo.VFlexBox,
	components: [
		{kind: "PageHeader", components: [
			{content: "Page Header"}
		]},
		{flex: 1, kind: "Pane", components: [
			{flex: 1, kind: "Scroller", components: [
				//Insert your components here
			]}
		]},
		{kind: "Toolbar", components: [
		]},
		{ kind: enyo.Control, name: "output", style: "background-color: LightSkyBlue; padding: 2px 6px;" },
		{ kind: enyo.Hybrid, name: "plugin", width: 0, height: 0, executable: "clip",
		  onPluginReady: "handlePluginReady", style: "float: left;" }
    ],
	pluginReady: false,
	create: function() {
		this.inherited(arguments);
		this.$.plugin.addCallback("testFunc", enyo.bind(this, "handleTestFunc"), true);
		this.somethingchanged();
	},
	somethingchanged: function() {
		var hellostr = "not the right string";
		if (this.pluginReady) {
			try {
				var status = this.$.plugin.callPluginMethod("hello_world", hellostr);
				this.outputMessage("X: " + JSON.stringify(status));
			}
			catch (e) {
				this.outputMessage("Plugin exception: " + e);
			}
		}
		else {
			this.outputMessage("plugin not ready");
		}
	},
	handleTestFunc: function(a1, a2) {
		this.outputMessage("plugin called testFunc() with arguments " + a1 + " and " + a2);
	},
	handlePluginReady: function(inSender) {
		this.outputMessage("plugin initalized");
		this.pluginReady = true;
	},
	outputMessage: function(message) {
		this.$.output.setContent(message);
		console.log("*** " + message);
	}
});
