require 'cairo'

local cs,display
local main_start=0
local degree2radian=0
local percent2radian=0
local one_sixth_of_circle=0
local global_oscillator=0

function oscillation_template()
	local counter=0
	return
		function (increament)
			counter=counter+increament
			return counter
		end
end

local app_data =
		{	
			['system']={	name='System', name_update_code='no update',
							cpu=0, cpu_update_code='${cpu cpu0}',
							mem=0, mem_update_code='${memperc}',
							x=100,
							y=100,
							r=80,
							oscillator=oscillation_template()
											},
			['top1']={		name='', name_update_code='${top name 1}',
							cpu=0, cpu_update_code='${top cpu 1}',
							mem=0, mem_update_code='${top mem 1}',
							x=300,
							y=200,
							r=60,
							oscillator=oscillation_template()
											},
			['top2']={		name='', name_update_code='${top name 2}',
							cpu=0, cpu_update_code='${top cpu 2}',
							mem=0, mem_update_code='${top mem 2}',
							x=450,
							y=200,
							r=60,
							oscillator=oscillation_template()
											},
			['top3']={		name='', name_update_code='${top name 3}',
							cpu=0, cpu_update_code='${top cpu 3}',
							mem=0, mem_update_code='${top mem 3}',
							x=600,
							y=200,
							r=60,
							oscillator=oscillation_template()
											},
			['top1m']={		name='', name_update_code='${top_mem name 1}',
							cpu=0, cpu_update_code='${top_mem cpu 1}',
							mem=0, mem_update_code='${top_mem mem 1}',
							x=300,
							y=400,
							r=60,
							oscillator=oscillation_template()
											},
			['top2m']={		name='', name_update_code='${top_mem name 2}',
							cpu=0, cpu_update_code='${top_mem cpu 2}',
							mem=0, mem_update_code='${top_mem mem 2}',
							x=450,
							y=400,
							r=60,
							oscillator=oscillation_template()
											},
			['top3m']={		name='', name_update_code='${top_mem name 3}',
							cpu=0, cpu_update_code='${top_mem cpu 3}',
							mem=0, mem_update_code='${top_mem mem 3}',
							x=600,
							y=400,
							r=60,
							oscillator=oscillation_template()
											}					
		}

function conky_init()
	main_start=0
	degree2radian=math.pi/180
	percent2radian=math.pi/50
	one_sixth_of_circle=math.pi/3

	-- cancel runtime tonumber bug
	for i,v in pairs(app_data) do
		if v.name_update_code~='no update' then conky_parse(v.name_update_code) end
		tonumber(conky_parse(v.cpu_update_code))
		tonumber(conky_parse(v.mem_update_code))
	end

end

function show_text(x,y,content)
	cairo_move_to(display,x,y)
	cairo_show_text(display,content)
	cairo_stroke (display)
end

function draw_pendulum(x,y,r,tilted_angle,angular_width)
	local thickness=r/4
	tilted_angle=tilted_angle+math.pi/2
	cairo_set_line_width (display,thickness)
	cairo_set_source_rgba (display, 0.2, 0.8, 0.2, 0.4)
	cairo_move_to(display,x,y)
	cairo_arc (display, x, y, r-thickness, tilted_angle-angular_width/2, tilted_angle+angular_width/2)
	cairo_fill (display)

end

function draw_sring(x,y,r,tilted_angle,angular_width)
	if angular_width < 0.1 then angular_width=0.1 end
	local thickness=r/3
	local begin_angle,end_angle=tilted_angle-angular_width/2,tilted_angle+angular_width/2

	cairo_set_line_width (display,thickness)

-- the arc
	cairo_set_source_rgba (display, 0.2, 0.8, 0.2, 0.4)
	cairo_arc (display, x, y, r, begin_angle, end_angle)
	cairo_stroke (display)

-- background ring
	cairo_set_source_rgba (display, 1, 1, 1, 0.4)
	cairo_arc (display, x, y, r, end_angle, begin_angle)
	cairo_stroke (display)

end

function draw_rotating_3_arc_block(x,y,r,tilted_angle)
	local thickness=r/3
	cairo_set_line_width (display,thickness)
	cairo_set_source_rgba (display, 0.2, 0.8, 0.4+0.4*math.sin(0.5*tilted_angle), 0.4-0.14*math.sin(tilted_angle))

	cairo_arc (display, x, y, r, tilted_angle, tilted_angle+one_sixth_of_circle)
	cairo_stroke (display)

	tilted_angle=tilted_angle+2*one_sixth_of_circle
	cairo_arc (display, x, y, r, tilted_angle, tilted_angle+one_sixth_of_circle)
	cairo_stroke (display)

	tilted_angle=tilted_angle+2*one_sixth_of_circle
	cairo_arc (display, x, y, r, tilted_angle, tilted_angle+one_sixth_of_circle)
	cairo_stroke (display)
end

function show_app_data(v)
	local x,y,r=v.x,v.y,v.r
	draw_rotating_3_arc_block(x, y, r, 0.003*v.oscillator(v.cpu+1))
	draw_sring(x,y,r*0.58,-0.02*global_oscillator,v.mem*percent2radian)
	draw_pendulum(x,y,r*0.58,0.5*math.sin((v.mem+100)/200*0.04*global_oscillator),0.4)
	show_text(x-20,y,v.name)
end

function conky_main()

	if main_start==1 then
		global_oscillator=global_oscillator+1

		for i,v in pairs(app_data) do
			
			if global_oscillator % 50 == 0 then -- update data in longer intervals
				if v.name_update_code ~= 'no update' then v.name=conky_parse(v.name_update_code) end
				v.cpu=tonumber(conky_parse(v.cpu_update_code))
				v.mem=tonumber(conky_parse(v.mem_update_code))
			end
			
			show_app_data(v)
			
		end

	else
		if tonumber(conky_parse('${updates}')) > 5 then
			main_start=1
			cs=cairo_xlib_surface_create(conky_window.display, conky_window.drawable, conky_window.visual, conky_window.width, conky_window.height)
			display=cairo_create(cs)
		end
	end

end

function conky_end()
	cairo_destroy(display)
	cairo_surface_destroy(cs)
end

