#include <MFL/MFL.h>
#include <MOGL/MOGL.h>
#include <MWL/MWL.h>
#include <Monk/Monk.h>

#include <iostream>
#include <assert.h>

mml::vec2 p0 = mml::vec2(100, 100);
mml::vec2 p1 = mml::vec2(200, 200);
mml::vec2 control = mml::vec2(150, 400);

int keyDown = (int)mwl::KeyCode::D1;

uint32_t mouseX, mouseY;

bool flag = false;

void OnMouseButtonDown(const mwl::MouseButtonDownEvent& e)
{
	if (e.Button == mwl::MouseButton::Left)
		p0 = mml::vec2(e.X, e.Y);
	else if (e.Button == mwl::MouseButton::Right)
		p1 = mml::vec2(e.X, e.Y);
	else if (e.Button == mwl::MouseButton::Middle)
		control = mml::vec2(e.X, e.Y);

	flag = true;

	std::cout << "x: " << e.X << ", y: " << e.Y << std::endl;
}

void OnMouseMove(const mwl::MouseMovedEvent& e)
{
	mouseX = e.X;
	mouseY = e.Y;
}

void OnKeyDown(const mwl::KeyDownEvent& e)
{
	if (e.Key == mwl::KeyCode::F1)
		keyDown--;
	else if (e.Key == mwl::KeyCode::F2)
		keyDown++;
	else
		keyDown = (int)e.Key;
}

static float Lerp(float start, float end, float t)
{
	return start + (end - start) * t;
}

static mml::vec2 BezierInterpolation(mml::vec2 p0, mml::vec2 p1, mml::vec2 p2, float t)
{
	float intAX = Lerp(p0.x, p1.x, t);
	float intAY = Lerp(p0.y, p1.y, t);

	float intBX = Lerp(p1.x, p2.x, t);
	float intBY = Lerp(p1.y, p2.y, t);

	mml::vec2 res;
	res.x = Lerp(intAX, intBX, t);
	res.y = Lerp(intAY, intBY, t);

	return res;
}

int main()
{
	//monk::Ref<MFL::Font> font = monk::CreateRef<MFL::Font>("C:/Users/kamil/OneDrive/Рабочий стол/CascadiaCode.ttf");
	//monk::Ref<MFL::Font> font = monk::CreateRef<MFL::Font>("C:/Windows/Fonts/arial.ttf");
	//monk::Ref<MFL::Font> font = monk::CreateRef<MFL::Font>("C:/Windows/Fonts/calibri.ttf");
	monk::Ref<MFL::Font> font = monk::CreateRef<MFL::Font>("C:/Windows/Fonts/segoeprb.ttf");
	

	mwl::WindowProps windowProps;
	windowProps.OpenGLContextVersion = mwl::OpenGLVersion::OPENGL_4_6;
	mwl::Window* window = mwl::Create(windowProps);
	window->SetFullscreen(false);

	if (mogl::OpenGLLoader::LoadOpenGL(mogl::OpenGLVersion::OPENGL_4_6) != mogl::OpenGLVersion::OPENGL_4_6)
		assert(false && "failed to load opengl");

	const MFL::TTF& ttf = font->GetTTF();
	uint32_t unicode_char = '1';


	monk::Renderer2D renderer;
	renderer.PushFont(font);

	monk::Render::EnableBlending(true);
	monk::Render::EnableDepthTest(true);

	window->SetMouseButtonDownCallback(OnMouseButtonDown);
	window->SetKeyDownCallback(OnKeyDown);
	window->SetMouseMovedCallback(OnMouseMove);

	while (!window->Closed())
	{
		window->Update();

		monk::Render::SetClearColor(0.55f, 0.55f, 0.55f, 1.0f);
		monk::Render::SetClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		monk::Render::Clear();

		//monk::Render::Viewport(glyf.x_min, glyf.y_min, (glyf.x_max - glyf.x_min), (glyf.y_max - glyf.y_min));
		monk::Render::Viewport(window->GetWidth(), window->GetHeight());

		//monk::Ref<monk::OrthographicCamera> camera = monk::CreateRef<monk::OrthographicCamera>(glyf.x_min, (glyf.x_max - glyf.x_min) , glyf.y_min, (glyf.y_max - glyf.y_min), -1, 1);
		monk::Ref<monk::OrthographicCamera>camera = monk::CreateRef<monk::OrthographicCamera>(0, window->GetWidth(), window->GetHeight(), 0, -1, 1);
		renderer.Begin(camera);

#if 0
		for (size_t i = 0; i < contours.size(); i++)
		{
			for (size_t ii = 0; ii < contours[i].points.size(); ii++)
			{
				MFL::GlyfPoint& point = contours[i].points[ii];

				const int size = 10;

				const int offset = 0;

				p0.x += offset;
				p0.y += offset;
				p1.x += offset;
				p1.y += offset;
				control.x += offset;
				control.y += offset;

				mml::vec4 color = point.on_curve ? mml::vec4(1.0f, 0.0f, 0.0f, 1.0f) : mml::vec4(0.0f, 1.0f, 1.0f, 1.0f);


				//renderer.DrawBezierCurve(mml::vec2(p0.x, p0.y), mml::vec2(p1.x, p1.y), mml::vec2(control.x, control.y), mml::vec4(1.0f, 0.0f, 0.0f, 1.0f), 32, 4);
				renderer.DrawQuad(mml::vec2(point.x - size / 2, point.y - size / 2 + 200), mml::vec2(size), color);
			}
		}
#elif 0

		//renderer.DrawBezierCurve(p0, p1, control, mml::vec4(1.0f, 0.0f, 0.0f, 1.0f), 8, 32);

		const size_t x = 10;
		const size_t y = 10;
		const size_t w = window->GetWidth() / x;
		const size_t h = window->GetHeight() / y;

		for (size_t i = 0; i < y; i++)
		{
			for (size_t j = 0; j < x; j++)
			{
				renderer.DrawQuad(mml::vec2(j * w, i * h), mml::vec2(w, h), mml::vec4((float)j / x, (float)i / y, 0.5f, 1.0f));
				//renderer.DrawCircle(mml::vec2(j * w, i * h), 25, mml::vec4((float)j / x, (float)i / y, 0.5f, 1.0f));
			}
		}

		//renderer.DrawQuad(mml::vec2(100, 100), mml::vec2(250, 123), mml::vec4(1.0f, 0.0f, 1.0f, 1.0f));

		renderer.DrawCircle(p0, 100, mml::vec4(0.0f, 1.0f, 0.0f, 1.0f));

#elif 1
		//const auto& atlasTexture = font->GetAtlas().m_AtlasTextures[keyDown - ' '];
		const auto& atlasTexture = font->GetAtlas().m_AtlasTextures[keyDown - '1'];
		monk::Ref<monk::Texture2D> texture = monk::Texture2D::Create(atlasTexture.Width, atlasTexture.Height, monk::TextureFormat::RED, atlasTexture.Buffer.data());

		const MFL::Glyf& glyf = ttf.glyfs[ttf.cmap[(int)keyDown]];
		const MFL::Path path = GetGlyphPath(glyf);

		renderer.DrawTexture(mml::vec2(glyf.x_min, glyf.y_min), texture->Size(), texture);

		renderer.DrawQuad(mml::vec2(mouseX, mouseY - 0.5f), mml::vec2(window->GetWidth() - mouseX, 1.0f), mml::vec4(1.0f, 0.0f, 0.0f, 1.0f));

		if (flag && false)
		{
			std::optional<MFL::Intersection> closest = std::nullopt;

			for (const auto& contour : path)
			{
				std::optional<MFL::Intersection> intersection = RayContourIntersection(mouseX, mouseY, contour);

				if (!closest && intersection)
					closest = intersection;

				if (intersection && intersection->Distance < closest->Distance)
					closest = intersection;
			}

			if (closest)
			{
				std::cout << (int)closest->Dir << std::endl;
			}

			flag = false;
		}

		if (true)
		{
			for (const auto& contour : path)
			{
				for (const auto& curve : contour)
				{
					mml::vec2 p0(curve.P0.x, curve.P0.y);
					mml::vec2 p1(curve.P1.x, curve.P1.y);
					mml::vec2 p2(curve.P2.x, curve.P2.y);

					renderer.DrawCircle(p0, 5, mml::vec4(1.0f, 0.0f, 0.0f, 1.0f));
					renderer.DrawCircle(p1, 5, mml::vec4(0.0f, 1.0f, 0.0f, 1.0f));
					renderer.DrawCircle(p2, 5, mml::vec4(1.0f, 0.0f, 0.0f, 1.0f));

					for (float t = 0; t <= 1.0f; t += 0.001f)
					{
						mml::vec2 point = BezierInterpolation(p0, p1, p2, t);

						renderer.DrawCircle(point, 2, mml::vec4(0.0f, 0.0f, 1.0f, 1.0f));
					}

					std::optional<MFL::Intersection> intersection = RayCurveIntersection(mouseX, mouseY, curve);

					if (intersection)
					{
						renderer.DrawCircle(mml::vec2(50, 50), 10, mml::vec4(0.0f, 1.0f, 0.0f, 1.0f));

						mml::vec4 color = intersection->Dir == MFL::Direction::Clockwise ? mml::vec4(1.0f, 1.0f, 0.0f, 1.0f) : mml::vec4(1.0f, 0.0f, 1.0f, 1.0f);
						float radius = intersection->Dir == MFL::Direction::Clockwise ? 10 : 15;

						renderer.DrawCircle(mml::vec2(intersection->Distance, mouseY), radius, color);

						if (flag)
						{
							std::cout << (int)intersection->x << std::endl;
							std::cout << (int)intersection->y << std::endl;
							std::cout << (int)intersection->Dir << std::endl;

							flag = false;
						}
					}
				}
			}
		}

#endif
		renderer.End();

		window->SwapBuffers();
	}

	return 0;
}