-- 
-- This is the skeleton of a simple dissector for the SDDS protocol
--
-- How to use?
--   Put it in the "plugins" directory of your Wireshark installation.
--    Windows: %APPDATA%\Wireshark\plugins
--    MacOS:   INSTALLDIR/lib/wireshark/plugins
--    Linux:   $HOME/.config/wireshark/plugins

-- **
-- ** Preliminaries 
-- **

-- First, create the dissector "object" 
local MY_MATH_PROTO = Proto("MATH","MATH PROTO")

--
-- Define SDDS header fields to make life easier later on
--
-- The format here is:
--   ProtoField.{type}(abbr,[name],[desc],[base],[valuestring],[mask])
--        for {type} = uint8/16/24/32/64 or framenum
--   ProtoField.{type}(abbr,[name],[desc])
--        for {type} = float, double, string, bytes, bool, etc.

local F = MY_MATH_PROTO.fields
F.operation = ProtoField.string("math.operation", "Message type")


-- **
-- ** This is the "callback" where the dissector starts for each packet
-- **

function MY_MATH_PROTO.dissector(buffer,pinfo,tree)

    -- Add a protocol object to the packet display window & attach the buffer to it
        packet = tree:add(MY_MATH_PROTO, buffer(), "MATH Packet")

    
        local operandSizeArray = {0,0,0,0,0,0,0,0}
        local sizeArrayIndex = 0
        local operandSize = 0
        local bufferIndex = 1
        local haveOperand = 0
        local encounteredNonNumericChar = 0
        local numOfOperands = 0
        local foundSecondOp = 0
        local bufferLen = 256
        local nonNumericCharArray= {0,0,0,0,0,0,0,0}
        local nonNumericSizeCounter = 0
        local nonNumericIndex = 1
        local numberOfNonNumericStrings = 0
        local inputStartsWithNonNumericChar = 0
        local bufferSize = 0
        local hsize = 1
        local psize = bufferSize - hsize
        local poff = hsize
        local pnum = psize / 4

        --This is determining the number of operands privided and the size of each for when we mess with the payload
	   


       -- local variables
      

       -- **
       -- ** Change what's displayed in the "Info" section of the summary capture frame 
       -- **

       -- Label the protocol 
       pinfo.cols.protocol = "MATH"
       
       -- Add some info
       local tempHeader = buffer:raw(0,1)
       tempHeader = string.byte(tempHeader)
       if(tempHeader == 66 or tempHeader == 98) then
         pinfo.cols.info = "Message type: Bye "
         pinfo.cols.info:append(buffer(0,1):string())
         pinfo.cols.info:append(" Message: ")
         pinfo.cols.info:append(buffer(0):string())
       elseif(tempHeader == 69 or tempHeader == 101) then
         pinfo.cols.info = "Message type: Error "
         pinfo.cols.info:append(buffer(0,2):string())
         pinfo.cols.info:append(" Message: ")
         pinfo.cols.info:append(buffer(2):string())
       else
         pinfo.cols.info = "Operation: "
         pinfo.cols.info:append(buffer(0,1):string())
         pinfo.cols.info:append(" Operands: ")
         pinfo.cols.info:append(buffer(1):string())
       end

       

       -- Add packet size to header display in dissection frame
       hdr = packet:add(buffer(0,1),"Header (" .. tostring(1) .. " bytes)")
       
       -- **
       -- ** Change the way the header data is displayed in the dissection frame
       -- **

       hdr:add(F.operation,    buffer(0,1)) 

       --
       -- Note .. instead of using the more elegant "object" approach here with F.xyz,
       --   we could have used a more direct approach for each header element:
       --   hdr:add(buffer(2,2),"FrameID: " .. buffer(2,2):uint())
       --

       -- Add the payload bytes to the "packet" object
      
       
       local numOfOpsDisected = 0
       local lastValue = 0

       if(tempHeader == 66) then
         pay = packet:add(buffer(0), "Payload Message Data")
         local iq = pay:add(buffer(0), j)
         local II = buffer(0)
         iq:append_text("Message: ".. II:string() .."")
       elseif(tempHeader == 69) then
         pay = packet:add(buffer(hsize), "Payload Message Data")
         local iq = pay:add(buffer(hsize), j)
         local II = buffer(hsize)
         iq:append_text("Message: ".. II:string() .."")
       else
         pay = packet:add(buffer(hsize), "Payload Message Data")
         for counter = 0, buffer:len() - 2 do
            --if buffer:raw(bufferIndex,1) == nil then
            --   break
            --else
            local temp = buffer:raw(bufferIndex,1)
            temp = string.byte(temp)
            if((temp >= 48 and temp <= 57)) then
               if (encounteredNonNumericChar == 1) then --This will record the size of the non-numeric chars before the first operand in the array of non-numeric strings at position 0 for paylod printing
                  nonNumericCharArray[nonNumericIndex] = nonNumericSizeCounter
   
                  local iq = pay:add(buffer(poff, nonNumericSizeCounter), j)
                  local delimiter = buffer(poff, nonNumericSizeCounter)
                  iq:append_text("Delimiter: " .. delimiter:string() .. "")
                  poff = poff + nonNumericSizeCounter
                  
                  encounteredNonNumericChar = 0
                  nonNumericIndex = nonNumericIndex + 1
                  nonNumericSizeCounter = 0
                  numberOfNonNumericStrings = numberOfNonNumericStrings + 1
               end
               haveOperand = 1
               operandSize = operandSize + 1
                
            elseif((temp < 48 or temp > 57))then
               if(haveOperand == 1)then
                  operandSizeArray[sizeArrayIndex] = operandSize
                  local iq = pay:add(buffer(poff, operandSize), j)
                  local operand = buffer(poff, operandSize)
                  iq:append_text("Operand: " .. operand:string() .. "")
                  poff = poff + operandSize
                  operandSize = 0
                  sizeArrayIndex = sizeArrayIndex + 1
                  numOfOperands = numOfOperands + 1
                  haveOperand = 0
               end
               encounteredNonNumericChar = 1
               if(haveOperand == 0)then
                  inputStartsWithNonNumericChar = 1
               end
               nonNumericSizeCounter = nonNumericSizeCounter + 1
            else
            end
            bufferIndex = bufferIndex + 1
            bufferSize = bufferSize + 1
            
          end
          if(haveOperand == 1)then
            local iq = pay:add(buffer(poff,operandSize), j)
            local operand = buffer(poff, operandSize)
            iq:append_text("Operand: " .. operand:string() .. "")
          elseif(encounteredNonNumericChar == 1) then
            local iq = pay:add(buffer(poff, nonNumericSizeCounter), j)
            local delimiter = buffer(poff, nonNumericSizeCounter)
            iq:append_text("Delimiter: " .. delimiter:string() .. "")
          end
          bufferIndex = 1
          sizeArrayIndex = 0
          nonNumericIndex = 0
       end
       
       
      
      
      


       --packetCounter = packetCounter + 1
end
--end

--
-- Define an initialization function ("constructor")
-- This gets executed once at the beginning of the session
--
function MY_MATH_PROTO.init()
    --** Put something here if desired, like a packet counter
    --local packetCounter = 0
end

-- 
-- Register the dissector at UDP port 4991
--

tcp_table = DissectorTable.get("tcp.port")
tcp_table:add(3000,MY_MATH_PROTO)
