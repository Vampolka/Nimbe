/*
NimbeAudio.scd
Author: Émile Ouellet-Delorme
Last Edited: 07/03/18

Loads the samples produces the sound for Nimbe.
It creates many tracks of different synths that loop samples and transposes them to a desired pitch.
It receives OSC and chances the playback of the samples accordingly.
*/

Server.killAll;

//Boot the server
s = Server.local;
s.options.sampleRate= 48000;
s.options.numOutputBusChannels = 2;
s.boot;

//Quit the server
s.quit;

//Create the synths that will play the samples - for the tracks
SynthDef(\sf_play_spat, { arg level,pos_spat, buf_pos, bufnum, rate, lag;
    var sound;
    sound =
	((PlayBuf.ar(1, bufnum, VarLag.kr(rate, lag,0,\sin), 1, buf_pos, 1) * Lag.kr(level, lag)));
	Out.ar(0, PanAz.ar(2, sound, Lag.kr(pos_spat,lag)));
}).send(s);

//For the bass track
SynthDef(\playbuf, { arg out=0,bufnum=0;
	Out.ar(0,PlayBuf.ar(8, bufnum, BufRateScale.kr(bufnum)));
	Out.ar(2,PlayBuf.ar(8, bufnum, BufRateScale.kr(bufnum)));
	Out.ar(4,PlayBuf.ar(8, bufnum, BufRateScale.kr(bufnum)));
	Out.ar(6,PlayBuf.ar(8, bufnum, BufRateScale.kr(bufnum)));
}).send(s);

//load the samples
~listeDeSamples = List.new();

for (1, 50, { arg i;
	~listeDeSamples.add(Buffer.readChannel(s, format("D:\\Projet4-spinner\\sons/samp-0%.wav",i) , channels: [0]))
});

~bass0 = Buffer.read(s, "D:\\Projet4-spinner\\sons/BassPunch1.flac");

~dong = Buffer.read(s, "D:\\Projet4-spinner\\sons/Dong.wav");

a = List.new();

for(0,60,{arg i;
	a.add(List.new());
});

//OSC receivers
//Change the pitch or the pan of a track
OSCdef.new(\changePitchOrPan,  { arg msg;
    var transpo, synth_index, level, pan, lag,sample;
    synth_index = msg[1];
    transpo = msg[2];
	level = msg[3];
	pan = msg[4];
	lag = msg[5];
	sample = msg[6];

	t = Task({
		0.1.wait;
		a[sample-1][synth_index].set(\lag, lag);
		a[sample-1][synth_index].set(\rate, transpo);
		a[sample-1][synth_index].set(\level, level);
		a[sample-1][synth_index].set(\pos_spat, pan);
	}).play;
}, '/Change', recvPort: 5678);

//Change the sample used for a track
OSCdef.new(\changeSample,  { arg msg;
    var transpo, synth_index, level, pan, lag,sample;
    synth_index = msg[1];
    transpo = msg[2];
	level = msg[3];
	pan = msg[4];
	lag = msg[5]*4;
	sample = msg[6];

	t = Task({
		0.1.wait;
		a[sample-2][synth_index].set(\lag, lag);
		a[sample-2][synth_index].set(\level, 0);
		lag.wait;
		a[sample-2][synth_index].free;
	}).play;

	p = Task({
		a[sample-1].add(Synth.new(\sf_play_spat, [level: 0, pos_spat: pan, buf_pos: 0,
			bufnum: [~listeDeSamples[sample-1], ~listeDeSamples[sample], ~listeDeSamples[sample+1], ~listeDeSamples[sample+2], ~listeDeSamples[sample+3]].choose,
			rate: transpo, lag:lag]));
		0.05.wait;
		a[sample-1][synth_index].set(\level,level);
	}).play;


}, '/ChangeSample', recvPort: 5678);

//Create a new track
OSCdef.new(\newTrack,  { arg msg;
    var rate_msg, synth_index, pan, lag, sample, level;
	synth_index = msg[1];
    rate_msg = msg[2];
	pan = msg[3];
	lag =  80;
	sample = msg[4];
	level = msg[5];
	t = Task({
		a[sample-1].add(Synth.new(\sf_play_spat, [level: 0, pos_spat: pan, buf_pos: 0,
			bufnum: [~listeDeSamples[sample-1], ~listeDeSamples[sample], ~listeDeSamples[sample+1], ~listeDeSamples[sample+2], ~listeDeSamples[sample+3]].choose,
			rate: rate_msg, lag:lag]));
	0.05.wait;
	a[sample-1][synth_index].set(\level,level);
	}).play;
}, '/New', recvPort: 5678);

//Start the bass (the backing track)
OSCdef.new(\startBass,  { arg msg;
	Synth.new(\playbuf,[\out, 0, \bufnum, ~bass0.bufnum]);
}, '/BassPunch', recvPort: 5678);

