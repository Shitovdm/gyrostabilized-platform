function [ rd ] = reading()
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here
s = serial('COM4');	           % creating object for s COM1
set(s,'BaudRate',921600);% configuring the buad to 9600, rest are set to default
set(s,'InputBufferSize',24); 
set(s,'TimeOut',10); 

fopen(s);
for i = 1:100
    A = fread(s);
    angles_hex=dec2hex(A);
    b=cellstr(angles_hex);
    
    a1_hex=strcat(b(12),b(11),b(10),b(9));
    angle1_dec=hex2dec(a1_hex);
    angle1_bin=dec2bin(angle1_dec);
    fl = typecast(uint32(bin2dec(angle1_bin)), 'single');
    
    a2_hex=strcat(b(16),b(15),b(14),b(13));
    angle2_dec=hex2dec(a2_hex);
    angle2_bin=dec2bin(angle2_dec);
    f2 = typecast(uint32(bin2dec(angle2_bin)), 'single');
    
    a3_hex=strcat(b(20),b(19),b(18),b(17));
    angle3_dec=hex2dec(a3_hex);
    angle3_bin=dec2bin(angle3_dec);
    f3 = typecast(uint32(bin2dec(angle3_bin)), 'single');
    
%     f_hex=strcat(b(9),b(8),b(7),b(6));
%     freq_dec=hex2dec(f_hex);
%     freq_bin=dec2bin(freq_dec);
%     fr = typecast(uint32(bin2dec(freq_bin)), 'single');
%     
%         PID1=strcat(b(13),b(12),b(11),b(10));
%     PID1=hex2dec(PID1);
%     PID1=dec2bin(PID1);
%     PID1 = typecast(uint32(bin2dec(PID1)), 'single');
    
    angle1(i)=fl;
    angle2(i)=f2;
    angle3(i)=f3;
%     freq(i)=fr;
%     PID(i)=PID1;
end
figure();
plot(angle1);



%subplot(2, 1, 2),plot(freq);
%drawnow
 while angle1(100)<100000

      A = fread(s);
      if A(1)==255
          angle_hex=dec2hex(A);
          b=cellstr(angle_hex);
          a1_hex=strcat(b(12),b(11),b(10),b(9));
          angle1_dec=hex2dec(a1_hex);
          angle1_bin=dec2bin(angle1_dec);
          fl = typecast(uint32(bin2dec(angle1_bin)), 'single');     
          
          a2_hex=strcat(b(16),b(15),b(14),b(13));
          angle2_dec=hex2dec(a2_hex);
          angle2_bin=dec2bin(angle2_dec);
          f2 = typecast(uint32(bin2dec(angle2_bin)), 'single');
          
          a2_hex=strcat(b(20),b(19),b(18),b(17));
          angle2_dec=hex2dec(a2_hex);
          angle2_bin=dec2bin(angle2_dec);
          f3 = typecast(uint32(bin2dec(angle2_bin)), 'single');
          for j=1:99
              angle1(j)=angle1(j+1);
              angle2(j)=angle2(j+1);
              angle3(j)=angle3(j+1);
          end
          angle1(100)=fl;
          angle2(100)=f2;
          angle3(100)=f3;
          
          %plot(angle1,angle2,angle3);
          %figure;
          plot(angle2);
          %plot(angle2);

          drawnow
         % subplot(2, 1, 2),plot(freq);
         % drawnow
      else
          fclose(s);
          fopen(s);
      end
  end
fclose(s);
end

