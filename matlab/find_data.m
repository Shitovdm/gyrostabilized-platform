function [ rd ] = reading()
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here
s = serial('COM4');	           % creating object for s COM1
set(s,'BaudRate',921600);% configuring the buad to 9600, rest are set to default
set(s,'InputBufferSize',24); 
set(s,'TimeOut',10); 

fopen(s);
% for i = 1:100
%     A = fread(s);
%     angles_hex=dec2hex(A);
%     b=cellstr(angles_hex);
%     
%     a1_hex=strcat(b(12),b(11),b(10),b(9));
%     a2_hex=strcat(b(20),b(19),b(18),b(17));
% 
%     fl = typecast(uint32(hex2dec(a1_hex)), 'single');
%     f2 = typecast(uint32(hex2dec(a2_hex)), 'single');
%     %fl = typecast(uint16(bin2dec(angle1_bin)), 'uint16');% —четчик данных
%     %fl = typecast(uint8(bin2dec(angle1_bin)), 'uint8');
%     angle1(i)=fl;
%     angle2(i)=f2;
% end
% 
% x = linspace(0,100);
% plot(x,angle1,x,angle2);

figure();
angle1 = [];
angle2 = [];
angle3 = [];
x = [];
counter = 1;

 while counter<1000
      A = fread(s);
      if A(1)==255
        angles_hex=dec2hex(A);
        b=cellstr(angles_hex);
    
        a1_hex=strcat(b(12),b(11),b(10),b(9));
        a2_hex=strcat(b(16),b(15),b(14),b(13));
        a3_hex=strcat(b(20),b(19),b(18),b(17));

        fl = typecast(uint32(hex2dec(a1_hex)), 'single');
        f2 = typecast(uint32(hex2dec(a2_hex)), 'single');
        f3 = typecast(uint32(hex2dec(a3_hex)), 'single');
        
        angle1(counter)=fl;
        angle2(counter)=f2; 
        angle3(counter)=f3;
        
        x(counter) = counter;
        
        plot(x,angle1,x,angle2,x,angle3);
        drawnow
        counter = counter+1; 
      else
          fclose(s);
          fopen(s);
      end
  end
fclose(s);
end

