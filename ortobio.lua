function ortobio_init()
    function sleep(s)
      local ntime = os.time() + s
      repeat until os.time() > ntime
    end

    if net.setapn({apn='wap.postemobile.it'})~=0 then sys.schedule(5*60); return end

    net.ntptime(1)

    sleep(20)

    function dummy_h(hdr)
          print("HTTPS header: "..hdr)
    end

--    https.on("header", dummy_h)
--    https.on('header')
    function dummy_r(state, data, len, more)
          print("HTTPS response: "..state .. " data: " .. data)
    end

--    https.on("response", dummy_r)
--    https.on('response')
end

function ortobio()

    sleep_interval=1
    herokuurl2='http://greenbeemfr.herokuapp.com/rest-api/controlunit/'
    herokuurl='http://muglab.uniroma3.it:8282/rest-api/controlunit/'
    firebaseurl='https://bioortoprova.firebaseio.com/controlunit.json'


    response={
        name=nil,
        battery_lvl=0,
        luminosity =0,
        ble_servers= {
        {
            id_ble         ='aabbccddeeff',
            _service       ='10203040506070809000a0b0c0d0e0f0',
            _characteristic='a0b0c0d0e0f010203040506070809000',
            reachable   ='no',
            temperature = 0,
            humidity_gnd= 0,
            battery_lvl = 0,
            humidity_air= 0,
            ph        = 0
        },
        {
            id_ble         ='102030405060',
            _service       ='010203040506070809000a0b0c0d0e0f',
            _characteristic='0a0b0c0d0e0f01020304050607080900',
            reachable   ='yes',
            temperature = 18,
            humidity_gnd= 80,
            battery_lvl = 40,
            humidity_air= 20,
            ph        = 6.3
        }

        },
    }


    working, response.name, _ = sms.siminfo()

    response.battery_lvl= sys.battery();

    if working~=1 then sys.schedule(5*60); return end



    --[[    bt.start('ortobio')  
        res = ortobio.acquire(sensors)
    --]]


    istant_t= os.date('*t', os.time())
    istant_norm= (istant_t.hour*60*60+istant_t.min*60+istant_t.sec)/(24*60*60)
    istant_hum_gnd=(istant_norm+istant_t.wday)/7
    istant_batt_lvl=(istant_t.day+istant_norm)/30
        
    response.ble_servers[2].temperature=((math.sin(istant_norm*2*3.1415 - 3.1415/2) + 1)/2 +sys.random(100)/200 - 100/400)*(20-10) + 10
    response.ble_servers[2].humidity_gnd=((-istant_hum_gnd-math.floor(-istant_hum_gnd)) +sys.random(100)/400)*(100)
    response.ble_servers[2].battery_lvl=((-istant_batt_lvl-math.floor(-istant_batt_lvl)) +sys.random(100)/400)*(100)
    response.ble_servers[2].humidity_gnd=((math.sin(istant_norm*2*3.1415 + 3.1415/2 + 0.3) + 1)/2 +sys.random(100)/200 - 100/400)*(100)




    while https.getstate()~=0 do end

    response_json = cjson_safe.encode(response)

    if response_json == nil then sys.schedule(5*60); return end

    res_http_hero=https.post(herokuurl, response_json)

--    sleep(10)

    res_http_fire=https.post(firebaseurl, response_json)
    res_http_hero=https.post(herokuurl2, response_json)

    sys.schedule(5*60); return
    
end
