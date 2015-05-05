class Equation 
  def self.solve_quadratic(a, b, c) # ax^2 + bx + c = 0
	solution = Array.new

	if a == 0 
		solution[0] = -c.to_f / b
		solution = nil if b == 0	# a == 0, b == 0
	else
		discriminant = b*b - 4*a*c
		solution[0] = (-b + discriminant) / (2*a).to_f
		
		solution.push((-b - discriminant) / (2*a).to_f) if ( discriminant > 0 ) # D > 0, 2 riesenia
		solution = nil if ( discriminant < 0 )
	end

	return solution
  end
end