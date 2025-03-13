clear;
clc;
dt = 0.01; %time step
t = 0:dt:10; %vector of all times
vi =10;
angle = 17.5;
anglerad = angle*pi()/180;

p = [0 0.4]; %initial position
ac = [0 -9.81]; %constant acceleration
ad = [0 0]; %drag acceleration
al = [0 0]; % lift acceleration
m = 0.175; %mass
cd0 = 0.08; %drag coefficient
cl0 = 0.15;
cdalpha = 2.72/2;
clalpha = 1.4;
rho = 1.2; %density of air
r = 0.274/2;
area = pi()*r^2;

for j=1:1:length(vi)
    v = [vi(j)*cos(anglerad) vi(j)*sin(anglerad)]; %initial velocity (m/s)
    for i=2:1:length(t)
                                                                                v(i, :) = v(i-1, :)+(ac+ad+al)*dt; %new vel = old vel + deltaT*accel
                                                                                p(i, :) = p(i-1, :)+v(i-1, :)*dt; %new pos = old pos + deltaT*vel

                                                                                theta = atan2((p(i, 2)-p(i-1, 2)), (p(i, 1)-p(i-1, 1))); %instantaneous angle of motion
                                                                                vtot = sqrt(v(i, 1)^2 + v(i, 2)^2); %instantaneous velocity

                                                                                alpha = anglerad - theta; %instantaneous angle of attack (AOA)
                                                                                cd = cd0 + cdalpha*(alpha-(-4*pi()/180))^2; %drag coefficient (including AOA)
                                                                                cl = cl0 + clalpha*alpha; %lift coefficient (including AOA)

                                                                                fd = 0.5*rho*(vtot^2)*cd*area; %drag force
                                                                                ad = (-fd/m)*[cos(theta) sin(theta)]; %acceleration due to drag (applies opposite to motion)      

                                                                                fl = 0.5*rho*(vtot^2)*cl*area; %lift force
                                                                                al = (fl/m)*[-sin(theta) cos(theta)]; % acceleration due to lift (applied perp. to motion)

                                                                                if(p(i, 2) < 0) %stop once the ball hits the "ground"
            airtime(j) = i*dt;
            peak(j) = max(p(:, 2));
            dist(j) = p(i, 1);
            break;
        end
    end
    hold on
    plot(p(:, 1), p(:, 2));
    legendInfo{j} = [num2str(vi(j)) ' m/s'];
end

hold off
legend(legendInfo);
title('Trajectory for Changing Initial Velocity');
subtitle('0.5m Initial Height, 10° Launch Angle');
xlabel('Horizontal Distance (m)');
ylabel('Vertical Distance (m)');
axis equal;
xlim([0 35]);
ylim([0 10]);


disp(airtime);
disp(peak);
disp(dist);