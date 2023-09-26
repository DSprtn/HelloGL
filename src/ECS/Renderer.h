class Renderer
{
public:
	public Renderer(std::string ModelPath);

	Draw();
	LoadMesh();

private:
	Model model;

};