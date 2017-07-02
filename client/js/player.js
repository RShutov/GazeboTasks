var ros = new ROSLIB.Ros();
  ros.connect('ws://' + document.URL.slice(5,-5) + "9090");

  var ForwardVelocity = 80;
  var BackwardVelocity = 25;

  var pubLeft = new ROSLIB.Topic({
    ros : ros,
    name : '/pioneer2dx/vel_left_cmd',
    messageType : 'std_msgs/Float32'
  });

  var pubRight = new ROSLIB.Topic({
    ros : ros,
    name : '/pioneer2dx/vel_right_cmd',
    messageType : 'std_msgs/Float32'
  });
  

  function publishEncoded(topic, obj) {
  var msg = new ROSLIB.Message({
    data: JSON.stringify(obj)
  });
  topic.publish(msg);
}
  
  var listener = new ROSLIB.Topic({
    ros : ros,
    name : '/pioneer2dx/vel_left_cmd',
    messageType : 'std_msgs/Float32'
  });

var listener2 = new ROSLIB.Topic({
    ros : ros,
    name : '/pioneer2dx/vel_right_cmd',
    messageType : 'std_msgs/Float32'
  });

  listener.subscribe(function(message) {
    console.log('Received message on ' + listener.name + ': ' + message.data);
  });
listener2.subscribe(function(message) {
    console.log('Received message on ' + listener2.name + ': ' + message.data);
  });

document.addEventListener('keydown', function(event) {
    if(event.keyCode == "W".charCodeAt(0)) {
       pubLeft.publish(new ROSLIB.Message({data: ForwardVelocity }));
       pubRight.publish(new ROSLIB.Message({data: ForwardVelocity }));
    }
    else if(event.keyCode == "S".charCodeAt(0)) {
       pubLeft.publish(new ROSLIB.Message({data: -BackwardVelocity }));
       pubRight.publish(new ROSLIB.Message({data: -BackwardVelocity }));
    } else if(event.keyCode == "D".charCodeAt(0)) {
       pubLeft.publish(new ROSLIB.Message({data: ForwardVelocity }));
       pubRight.publish(new ROSLIB.Message({data: -ForwardVelocity }));
    } else if(event.keyCode == "A".charCodeAt(0)) {
       pubLeft.publish(new ROSLIB.Message({data: -ForwardVelocity }));
       pubRight.publish(new ROSLIB.Message({data: ForwardVelocity }));
    }
});

document.addEventListener('keyup',function(event) {
       pubLeft.publish(new ROSLIB.Message({data: 0.0 }));
       pubRight.publish(new ROSLIB.Message({data: 0.0 }));
});

