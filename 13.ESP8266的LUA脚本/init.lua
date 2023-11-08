station_cfg = {}
station_cfg.ssid = "xlu_2.4"
station_cfg.pwd  = "111222333+"

station_cfg.auto = false
station_cfg.save = false

x_id=65
mqtt_cfg = {}
mqtt_cfg.host      = "broker.emqx.io"
mqtt_cfg.port      = 1883
mqtt_cfg.clientid  = "alro_xxx".. x_id
mqtt_cfg.keepalive = 120
mqtt_cfg.username  = "AlvaRocha"
mqtt_cfg.password  = "aio_KO<safety edit>sXwbgtWCboCal"
-- 主题为:/topic/xx/{id}/{c|m}
-- 
x_topic="/topic/xx/".. x_id
m=nil
wifi.setmode(wifi.STATION)
wifi.sta.config(station_cfg)

iot_test = mqtt.Client(mqtt_cfg.clientid, mqtt_cfg.keepalive, mqtt_cfg.username, mqtt_cfg.password)
iot_test:on("offline", function(client)
  print("client offline")
end)
iot_test:on("message", function(client, topic, data)
  --print("MQTT msg received on '" .. topic .. "':")
  if data ~= nil then
    print(data)
  end
end)

function get_broker(mqtt_client)
  mqtt_client:connect(mqtt_cfg.host, mqtt_cfg.port, false,
    function(client)
      client:subscribe(x_topic.."/c", 0, function(client)
        print("subscribe success "..x_topic.."/m")
      end)
         m:publish(x_topic.."/m", "success", 0, 0, function(client)
        print("init success")
      end)
    end,
    function(client, reason)
      print('connection failed', reason)
    end)
    m=mqtt_client
end


function startup()
  if file.open("init.lua") == nil then
    print("init.lua deleted or renamed")
  else
    print("Running")
    file.close("init.lua")
    get_broker(iot_test)
  end
end

wifi_connect_event = function(T)
  print("Connection to AP(" .. T.SSID .. ") established!")
  print("Waiting for IP address...")
  if disconnect_ct ~= nil then
    disconnect_ct = nil
  end
end

wifi_got_ip_event = function(T)
  -- Note: Having an IP address does not mean there is internet access!
  -- Internet connectivity can be determined with net.dns.resolve().
  print("Wifi connection is ready! IP address is: " .. T.IP)
  print("Startup will resume momentarily, you have 3 seconds to abort.")
  print("Waiting...")
  tmr.create():alarm(3000, tmr.ALARM_SINGLE, startup)
end

wifi.eventmon.register(wifi.eventmon.STA_CONNECTED, wifi_connect_event)
wifi.eventmon.register(wifi.eventmon.STA_GOT_IP, wifi_got_ip_event)
wifi.sta.connect()

 

uart.setup(0, 115200, 8, uart.PARITY_NONE, uart.STOPBITS_1)
uart.on("data","\n",function(data)
   m:publish(x_topic.."/m", data, 0, 0, function(client)
        print("sent")
      end)
end, 0)
