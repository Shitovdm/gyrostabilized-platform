function [ rd ] = reading()
% COM port settings.
s = serial('COM4');
set(s,'BaudRate',921600);
set(s,'InputBufferSize',24); 
set(s,'TimeOut',10); 

fopen(s);
% OX values.
counter = [];
for j=1:100
    counter(j) = j;
end
% Start 99 values.
for i = 1:100
    A = fread(s);
    angles_hex=dec2hex(A);
    b=cellstr(angles_hex);
    % Pitch.
    pitch_hex=strcat(b(12),b(11),b(10),b(9));
    fl = typecast(uint32(hex2dec(pitch_hex)), 'single');
    % Roll.
    roll_hex=strcat(b(16),b(15),b(14),b(13));
    f2 = typecast(uint32(hex2dec(roll_hex)), 'single');
    % Yaw.
    yaw_hex=strcat(b(20),b(19),b(18),b(17));
    f3 = typecast(uint32(hex2dec(yaw_hex)), 'single');
    
    pitch(i)=fl;
    roll(i)=f2;
    yaw(i)=f3;
end
figure();
plot(counter,pitch,counter,roll,counter,yaw);
% Refreshing plots.
   while 1==1
      A = fread(s);
      
      if A(1)==255
          angle_hex=dec2hex(A);
          b=cellstr(angle_hex);
          %strcat(b(1),b(2),b(3),b(4),b(5),b(6),b(7),b(8),b(9),b(10),b(11),b(12),b(13),b(14),b(15),b(16),b(17),b(18),b(19),b(20),b(21),b(22),b(23),b(24))
          pitch_hex=strcat(b(12),b(11),b(10),b(9));
          fl = typecast(uint32(hex2dec(pitch_hex)), 'single');     
          
          roll_hex=strcat(b(16),b(15),b(14),b(13));
          f2 = typecast(uint32(hex2dec(roll_hex)), 'single');
          
          yaw_hex=strcat(b(20),b(19),b(18),b(17));
          yaw_hex
          %typecast(uint32(hex2dec("3d383b98")), 'single')
          f3 = typecast(uint32(hex2dec(yaw_hex)), 'single');
          
          for j=1:99
              pitch(j)=pitch(j+1);
              roll(j)=roll(j+1);
              yaw(j)=yaw(j+1);
          end
          pitch(100)=fl;
          roll(100)=f2;
          yaw(100)=f3;
          
            % angle3 - yaw
            % angle2 - roll
            % angle1 - pitch
          % plot(counter,pitch,counter,roll,counter,yaw);
          %yaw
          plot(counter,yaw);
          %plot(counter,angle1);
          drawnow
      else
          fclose(s);
          fopen(s);
      end
  end
fclose(s);
fclose(instrfind);
end

