clear;
clc;
dt = 0.001; %time step
t = 0:dt:10; %vector of all times
vi = [12 15 18];
angle = 10;
anglerad = angle*pi()/180;

p = [0 0.5]; %initial position
ac = [0 -9.81]; %constant acceleration
ad = [0 0]; %drag acceleration
al = [0 0]; % lift acceleration
m = 0.175; %mass
cd = 0.1; %drag coefficient
cl = 0.175;
rho = 1.2; %density of air
r = 0.275/2;

for j=1:1:length(vi)
    v = [vi(j)*cos(anglerad) vi(j)*sin(anglerad)]; %initial velocity (m/s)
    for i=2:1:length(t)
        v(i, :) = v(i-1, :)+(ac+ad+al)*dt; %new vel = old vel + deltaT*accel
        p(i, :) = p(i-1, :)+v(i-1, :)*dt; %new pos = old pos + deltaT*vel

        theta = atan2((p(i, 2)-p(i-1, 2)), (p(i, 1)-p(i-1, 1))); %angle of current motion
        vtot = sqrt(v(i, 1)^2 + v(i, 2)^2); %total velocity

        fd = 0.5*rho*(vtot^2)*cd*pi()*(r^2); %drag force
        ad = (-fd/m)*[cos(theta) sin(theta)]; %drag force applies opposite to motion      
        
        fl = 0.5*rho*(vtot^2)*cl*pi()*(r^2); %lift force
        al = (fl/m)*[-sin(theta) cos(theta)]; % drag force applies perp. to motion

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
xlabel('Horizontal Distance (m)');
ylabel('Vertical Distance (m)');
axis equal;


disp(airtime);
disp(peak);
disp(dist);