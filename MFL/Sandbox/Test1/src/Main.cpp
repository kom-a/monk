#include <MFL/MFL.h>
#include <MOGL/MOGL.h>
#include <MWL/MWL.h>
#include <Monk/Monk.h>

#include <iostream>
#include <assert.h>

int main()
{
	mwl::WindowProps windowProps;
	windowProps.OpenGLContextVersion = mwl::OpenGLVersion::OPENGL_4_6;
	mwl::Window* window = mwl::Create(windowProps);
	window->SetCursor(mwl::Cursor("./oxy-bluecurve/oxy-bluecurve.inf"));

	if (mogl::OpenGLLoader::LoadOpenGL(mogl::OpenGLVersion::OPENGL_4_6) != mogl::OpenGLVersion::OPENGL_4_6)
		assert(false && "failed to load opengl");

	monk::Renderer2D renderer;
	monk::Render::EnableBlending(true);
	monk::Render::EnableDepthTest(true);

	monk::Ref<monk::OrthographicCamera> camera = monk::CreateRef<monk::OrthographicCamera>(0, window->GetWidth(), window->GetHeight(), 0, -1, 1);

	const std::string text = R"(
Lorem ipsum odor amet, consectetuer adipiscing elit. Sapien consequat morbi sagittis orci magna purus et fusce. Id pulvinar posuere vulputate molestie dapibus interdum curae parturient metus. Leo etiam ac fames mi vehicula. Venenatis quam ornare fusce; semper ornare iaculis ultrices nec. Nam ex tellus tellus ante faucibus.

Aquam sit suspendisse aptent risus urna sapien porttitor ipsum. In maximus phasellus maecenas id volutpat. Egestas molestie blandit tellus gravida pulvinar orci proin diam dapibus. Porttitor primis ligula maecenas sem quam nullam ex auctor. Efficitur natoque mollis egestas viverra dictum odio. Mollis pulvinar natoque aliquam massa accumsan. Metus ut at mauris sagittis varius orci magna euismod ad. Dui potenti ultrices nec quisque libero maecenas sapien quis.

Nisi potenti massa eros malesuada mauris. Placerat cras ex eleifend vitae enim sodales condimentum. Nisi diam dictum ac facilisi erat lacus aptent. Interdum ullamcorper magnis senectus mattis proin rutrum nulla, euismod tincidunt. Quisque rhoncus commodo adipiscing turpis; elementum nisl ante elit. Semper dui mollis laoreet platea tristique facilisis. Fringilla vitae duis efficitur eros cras iaculis.

Dolor aliquet elementum rutrum nulla curae a. Inceptos suspendisse nostra eu, placerat ornare varius bibendum. Ridiculus lacus vulputate sed duis augue aliquam torquent amet. Duis cubilia himenaeos torquent consectetur odio ex finibus nisl. Conubia duis cras potenti vitae nascetur vestibulum. Condimentum aptent litora urna aenean lectus fringilla; mus nec. Suscipit felis senectus condimentum neque vestibulum orci fringilla senectus. Euismod sapien leo quis consequat a sagittis vivamus.

Primis ut quis nisl facilisis maecenas ullamcorper pulvinar. Dignissim a dolor ridiculus et tellus. Nunc facilisis fusce fames risus porttitor nisi. Accumsan condimentum libero nullam venenatis maecenas natoque posuere augue lacinia. Gravida consequat nascetur laoreet tincidunt fringilla aliquam magnis per. Ullamcorper maximus metus, lacinia lectus vulputate ultrices. Fusce integer etiam molestie proin eleifend euismod eros. Lobortis mattis massa leo natoque adipiscing donec. Feugiat est in senectus primis interdum litora cubilia sociosqu. Faucibus pharetra nascetur consectetur curae nisl gravida aptent.

Risus tortor litora quisque metus ullamcorper pretium ligula. Sit condimentum himenaeos sit efficitur; quam vivamus fames semper gravida. Adipiscing curae parturient eget eleifend netus varius. Curae adipiscing litora fames varius quisque aenean eu rhoncus. At cras fringilla nulla eget adipiscing maecenas. Alibero rutrum ullamcorper turpis quisque facilisi congue ullamcorper. Aliquam lacinia mattis quis congue ante rutrum donec augue tortor. Ante nunc dignissim, imperdiet laoreet nulla est. Nostra efficitur elit consequat egestas ornare ornare. Egestas molestie posuere ad mattis dui.

Fringilla tortor ultrices pulvinar semper maecenas magna. Blandit bibendum venenatis nulla iaculis platea. Mus faucibus per fringilla luctus nascetur libero. Lacus etiam pellentesque ridiculus nascetur tincidunt conubia orci. Habitasse vulputate potenti ad sapien orci tempus risus. Nec sapien commodo donec in turpis ad. Imperdiet condimentum cras nec class ante ac. Quam at etiam congue eget blandit fermentum pretium fermentum tristique. Facilisi est nunc feugiat malesuada; nam ex gravida pretium.

Venenatis nisl pharetra odio ut eros, leo ultrices amet. Adipiscing ac egestas venenatis dolor montes libero condimentum arcu. Nec blandit quam odio ligula; mollis velit integer dapibus. Tellus adipiscing metus commodo leo nisi dolor. Pellentesque euismod fermentum et, sit mattis enim. Tincidunt gravida sollicitudin tortor lacus ullamcorper erat facilisi interdum sodales. Aliquam semper erat habitasse feugiat inceptos viverra bibendum aenean.

Tempus finibus adipiscing interdum habitant maecenas faucibus? Rhoncus erat efficitur sit pulvinar pulvinar urna. Ad risus rhoncus vulputate, etiam est maximus. Nibh consequat quis dictumst primis sem. Accumsan urna eu torquent metus cubilia. Senectus libero fusce porta rutrum porta justo sodales varius netus. Duis nullam nostra vel fusce vel magnis quam.

Non praesent quisque maximus hac nibh sapien ante quam. Vehicula dui aptent nec taciti accumsan dignissim erat felis hendrerit. Orci aliquet varius nam; blandit imperdiet vivamus aenean. Eleifend id ullamcorper mauris nec libero taciti phasellus rhoncus fames. Quisque consequat vitae netus dictumst hac aptent. Purus cursus lectus, rutrum metus magna at. Curabitur conubia accumsan non cubilia dolor id placerat conubia. Per tellus venenatis fermentum dolor vivamus. Lacus conubia congue per vulputate tempus.

Scelerisque bibendum platea nulla nunc sapien dui fames. Neque vitae hac efficitur ridiculus ligula ullamcorper faucibus. Pretium tortor condimentum donec nisi viverra porttitor a libero. Eleifend magnis est commodo dictumst varius ullamcorper per donec? Quam nec himenaeos felis nullam mollis at elit ligula suspendisse. Faucibus donec felis ultrices primis aliquam neque tincidunt tellus. Auctor accumsan volutpat et non gravida semper ligula. Aliquam ipsum fringilla finibus diam sociosqu interdum pretium facilisi.

Fusce malesuada ante interdum ridiculus quisque phasellus. Tristique himenaeos convallis dui egestas donec dolor a turpis. Nunc iaculis nunc class, sit viverra nisl. Sociosqu turpis class penatibus id bibendum ac. Elit odio cubilia nulla convallis feugiat laoreet maximus? Inceptos ut libero massa facilisi duis netus risus. Commodo ex sociosqu nibh; rutrum erat eu condimentum aenean in. Blandit a congue ornare molestie commodo efficitur.

Felis fames ac justo ut tempus etiam. Mus ex cursus facilisis lectus; arcu nam himenaeos. Lacinia ridiculus suscipit efficitur malesuada risus praesent phasellus. Justo malesuada commodo montes tempor placerat. Pharetra viverra penatibus maecenas diam finibus ultricies. Torquent pretium proin auctor justo euismod. Donec cras sollicitudin adipiscing tellus et torquent luctus lacus. Fermentum montes faucibus et risus tristique magnis porta. Mattis platea vivamus metus ut maecenas aliquet. Turpis efficitur dapibus, sollicitudin posuere facilisis felis.

Elit habitant ultricies leo; ridiculus morbi molestie. Tortor integer urna fames natoque accumsan netus malesuada. Quisque id porta, facilisi dis integer purus ad phasellus a. Rutrum imperdiet sollicitudin quis; convallis placerat suscipit duis conubia. Ridiculus placerat pretium nostra sem cubilia. Facilisis mollis sed egestas leo odio sapien maecenas. Asenectus sapien lacinia nullam; volutpat ipsum.

Nulla suspendisse vitae metus potenti taciti cursus. Ligula a mus cubilia placerat faucibus etiam potenti. Non sociosqu vel tempus ex feugiat maecenas. Congue nostra erat ipsum maecenas habitasse praesent, natoque litora tortor. Efficitur urna nulla dis justo viverra pulvinar hendrerit placerat. Libero aptent primis purus in imperdiet tortor eros. Viverra gravida lacus lorem vivamus ipsum imperdiet. Vestibulum placerat penatibus; urna ultrices accumsan natoque vestibulum felis ac.

Molestie taciti pretium at vivamus quam non eu ornare. Felis dapibus viverra auctor ante mi euismod risus. Etiam euismod luctus placerat habitasse quis nisi. Sodales sodales fusce semper ligula arcu. Libero rutrum magnis, justo sodales dictum erat. Efficitur a gravida platea turpis eu eleifend habitasse.

Sapien luctus ultricies venenatis eros et. Nascetur semper adipiscing accumsan bibendum ante egestas etiam eget. Erat congue suspendisse sagittis neque quam amet condimentum. Pellentesque molestie primis in luctus pretium at cras. Viverra convallis vulputate laoreet penatibus posuere interdum maximus auctor cursus. Mattis interdum in vel conubia imperdiet taciti. Urna integer ultrices ridiculus iaculis vestibulum.

At odio justo arcu suspendisse vivamus scelerisque risus at. Interdum ornare fermentum convallis tellus eros nam orci. Tempor porttitor aliquet aliquam dapibus habitant id nascetur quis. Eleifend molestie enim dis semper quam. Fusce netus sagittis eleifend tortor arcu nunc in elit nascetur? Parturient hendrerit phasellus egestas ullamcorper cubilia cubilia sed. Risus conubia vitae nec maecenas lacus finibus. Turpis finibus facilisi nisl torquent sollicitudin fames maximus. Metus libero hendrerit dignissim molestie fringilla dictumst.

Dictum magna vitae ut in sollicitudin ultricies conubia sodales. Rhoncus tempus tempus velit dignissim odio vestibulum suspendisse condimentum. Enim porttitor auctor tristique mauris; vel dictumst. Fermentum mi mus augue id purus ante suscipit morbi. Imperdiet magnis fusce tempor viverra natoque class ex. Enim leo orci mattis etiam iaculis. Ad mollis sollicitudin tincidunt inceptos tincidunt nec dictum. Nisi tristique massa elementum hac ultrices aliquam. Molestie eros efficitur dui; nec aptent ornare dolor. Dapibus id velit per dolor nec nunc pretium praesent.

Praesent eleifend habitasse nec pellentesque torquent facilisi gravida efficitur nullam. Tortor conubia dui vivamus habitasse pulvinar commodo rutrum mauris integer. Posuere vitae euismod nisi luctus himenaeos nullam bibendum sed. Etiam nec aliquam elit platea sem penatibus convallis himenaeos. Placerat eros erat; habitasse habitasse viverra quisque. Nunc proin sed dictum dolor, fermentum ex risus amet netus. Vehicula aenean ad inceptos laoreet ligula faucibus habitasse. Pellentesque viverra cras accumsan sociosqu, justo tristique consectetur facilisi. Diam integer erat nunc amet mauris.

Faucibus ac sodales curabitur metus hendrerit ac. Egestas pretium sed quis interdum porta sagittis vulputate. Leo potenti molestie himenaeos egestas, erat vitae egestas ante. Congue curabitur volutpat justo ligula sociosqu condimentum conubia ex. Fames himenaeos aliquet lacinia faucibus convallis accumsan sit augue. Class id ad pretium platea felis metus in integer habitant. Pharetra faucibus vehicula habitasse natoque; accumsan morbi erat in nibh. Mi condimentum facilisis; nisi laoreet quam lobortis.

Nascetur vehicula praesent eget justo quam per dictumst odio. Ridiculus dis quisque maximus quisque senectus aenean aptent nunc. Lobortis commodo convallis proin; ridiculus pretium inceptos. Erat porttitor tristique inceptos nisi rutrum volutpat feugiat fusce ac. Ullamcorper nascetur consectetur urna fermentum enim. Neque mollis dictum commodo tortor aliquam sociosqu praesent. Id hendrerit tristique dui per vivamus turpis habitasse. Vulputate suscipit ligula aliquet turpis ullamcorper; gravida scelerisque nunc.

Porttitor amet tempor hendrerit risus magnis tristique; tellus vel aliquet? Nam luctus potenti euismod parturient pulvinar justo; primis in ridiculus. Molestie duis gravida facilisis congue gravida bibendum vel suscipit. Netus nibh sit himenaeos vitae diam ac. Pretium augue pulvinar euismod senectus donec venenatis pretium. Ipsum ridiculus tempor aptent, porta torquent ornare mattis amet. Morbi sem eget elementum dui consectetur sit fames ipsum.

Hendrerit erat elementum consequat eros platea id vivamus est quisque. Egestas nec varius etiam habitasse etiam velit pulvinar aenean. Posuere diam primis amet vivamus pellentesque? Egestas aenean pellentesque vestibulum odio dolor turpis imperdiet nisi parturient? Taciti curabitur euismod eleifend habitasse torquent consequat amet. Rhoncus quisque lobortis eleifend, felis imperdiet mauris. Natoque phasellus metus adipiscing conubia maecenas ut nunc.

Lacus consectetur nunc; nec rhoncus tellus est consectetur. Natoque sollicitudin sed vulputate ridiculus a morbi magnis. Ullamcorper eros et dictumst ullamcorper vivamus curabitur porttitor. Iaculis enim laoreet sem purus sagittis. Vehicula himenaeos tincidunt volutpat varius phasellus. Est praesent duis consectetur himenaeos aliquam habitasse. Est leo mauris leo massa mi mi viverra.

Nullam nisi convallis pellentesque odio risus vitae. Curabitur euismod ex integer dolor pretium ridiculus. Ad quam venenatis hac metus class platea malesuada consequat. Placerat ultricies purus integer adipiscing et amet. Primis dignissim ac eget pretium vulputate cubilia. Odio phasellus tristique dolor risus penatibus finibus condimentum varius. Eu risus molestie magna est vel maecenas. Dolor porta sapien platea eu eu! Parturient ad ultrices mi curabi
	)";

	monk::Time timer;

	float y = 0;

	while (!window->Closed())
	{
		window->Update();

		monk::Render::SetClearColor(0.65f, 0.05f, 0.05f, 1.0f);
		monk::Render::Clear();
		monk::Render::Viewport(window->GetWidth(), window->GetHeight());

		camera->SetProjection(0, window->GetWidth(), window->GetHeight(), 0, -1, 1);

		renderer.Begin(camera);

		renderer.DrawString(text, mml::vec2(10, y), 12, mml::vec4(0.0f, 1.0f, 0.0f, 1.0f));

		renderer.End();

		y = -timer.Elapsed() * 50;

		if (y <= -1000)
		{
			timer.Reset();
		}

		window->SwapBuffers();
	}

	return 0;
}