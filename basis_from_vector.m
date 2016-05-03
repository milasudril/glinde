function A=basis_from_vector(direction)
	u=direction;
	a=[u(3);u(1);u(2)];
	v=cross(u,a);
	v=v/norm(v);
	w=cross(u,v);
	A=[u,v,w];
end
