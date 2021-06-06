//
// Created by ryan1 on 09/03/2021.
//
#include <glad/glad.h>
#include "Resources/resourceType.hpp"
#include "Resources/graphicalResource.hpp"
#include "ECS/Components/light.hpp"

#include <editor.hpp>

using namespace Solid;
GL::Mesh::Mesh(MeshResource *_raw):
IMesh(_raw->Meshes.size())
{
    name = _raw->name;


	Meshes.resize(_raw->Meshes.size());
	for (int i = 0; i < _raw->Meshes.size(); ++i) {
		MeshResource::SubMesh& rawSub = _raw->Meshes.at(i);
		GL::Mesh::SubMesh& sub = Meshes.at(i);
		glGenVertexArrays(1, &sub.VAO);
		glGenBuffers(1, &sub.VBO);
		glGenBuffers(1, &sub.EBO);
		glBindVertexArray(sub.VAO);
		glBindBuffer(GL_ARRAY_BUFFER, sub.VBO);
		glBufferData(GL_ARRAY_BUFFER, rawSub.vertices.size() * 8 *sizeof(GLfloat), rawSub.vertices.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 *sizeof(GLfloat), (void*)0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 *sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 *sizeof(GLfloat), (void*)(6*sizeof(GLfloat)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sub.EBO);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER,  rawSub.indices.size()*sizeof(unsigned int), rawSub.indices.data(), GL_STATIC_DRAW);
		sub.numOfIndices = rawSub.indices.size();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

        //ANIM
        if(_raw->hadAnim)
        {
            glGenBuffers(1,&sub.animVBO);
            size_t bufferSize = 4*sizeof(GLint) + 4*sizeof(GLfloat);
            glBindBuffer(GL_ARRAY_BUFFER, sub.animVBO);
            glBufferData(GL_ARRAY_BUFFER, rawSub.animData.size() * bufferSize, rawSub.animData.data(), GL_STATIC_DRAW);
            glVertexAttribIPointer(3,4,GL_INT, bufferSize, (void*)offsetof(AnimData, boneIds));
            glVertexAttribPointer(4,4,GL_FLOAT,GL_FALSE, bufferSize, (void*)offsetof(AnimData, weights));
            glEnableVertexAttribArray(3);
            glEnableVertexAttribArray(4);
        }
	}
	glBindVertexArray(0);
	bIsInit = true;
}

void GL::Mesh::Init(MeshResource *_raw)
{
	if(bIsInit)
		return;

    name = _raw->name;


	Meshes.resize(_raw->Meshes.size());
	for (int i = 0; i < _raw->Meshes.size(); ++i)
	{
		MeshResource::SubMesh& rawSub = _raw->Meshes.at(i);
		GL::Mesh::SubMesh& sub = Meshes.at(i);
		glGenVertexArrays(1, &sub.VAO);
		glGenBuffers(1, &sub.VBO);
		glGenBuffers(1, &sub.EBO);
		glBindVertexArray(sub.VAO);
		glBindBuffer(GL_ARRAY_BUFFER, sub.VBO);
		glBufferData(GL_ARRAY_BUFFER, rawSub.vertices.size() * 8 *sizeof(GLfloat), rawSub.vertices.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 *sizeof(GLfloat), (void*)0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 *sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 *sizeof(GLfloat), (void*)(6*sizeof(GLfloat)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);



		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sub.EBO);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER,  rawSub.indices.size()*sizeof(unsigned int), rawSub.indices.data(), GL_STATIC_DRAW);
		sub.numOfIndices = rawSub.indices.size();

        //ANIM
        if(_raw->hadAnim)
        {
            size_t bufferSize = 4*sizeof(GLint) + 4*sizeof(GLfloat);
            glBindBuffer(GL_ARRAY_BUFFER, sub.animVBO);
            glBufferData(GL_ARRAY_BUFFER, rawSub.animData.size() * bufferSize, rawSub.animData.data(), GL_STATIC_DRAW);

            glEnableVertexAttribArray(3);
            glVertexAttribIPointer(3, 4, GL_INT, bufferSize, (const GLvoid*)0);
            glEnableVertexAttribArray(4);
            glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, bufferSize, (const GLvoid*)16);

        }
	}
    glBindVertexArray(0);
	bIsInit = true;
}

void GL::Mesh::DrawMesh()
{
	glEnable(GL_DEPTH_TEST);

	for (auto& subMesh : Meshes)
	{
		glBindVertexArray(subMesh.VAO);
		glBindBuffer(GL_ARRAY_BUFFER, subMesh.VBO);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,subMesh.EBO);
		glDrawElements(GL_TRIANGLES, subMesh.numOfIndices,GL_UNSIGNED_INT, nullptr);
	}
	glBindVertexArray(0);
}

void GL::Mesh::DrawMesh(const std::vector<MaterialResource *>& _list, Transform& _tr, Camera& _cam, Animation* _anim)
{
	glEnable(GL_DEPTH_TEST);

	for (int i = 0; i < Meshes.size(); ++i)
	{
	    uint textID = 0;
		SubMesh& subMesh = Meshes.at(i);
		const MaterialResource* mat = _list.at(i);
        std::shared_ptr<IShader> shader = nullptr;
		if(mat == nullptr)
		{
			mat = Engine::GetInstance()->resourceManager.GetDefaultMat();
			mat->GetDefaultshader()->SetMVP(_tr, _cam);
		}
		else
		{
            shader = mat->GetShader();
            if(shader == nullptr)
				mat->GetDefaultshader()->SetMVP(_tr, _cam);
			else
			{
				for(auto& value : mat->fields)
				{
					switch (value.type)
					{
						case MaterialResource::EShaderFieldType::BOOL:
							shader->SetBool(value.name.c_str(), value.b);
							break;
						case  MaterialResource::EShaderFieldType::INT:
							shader->SetInt(value.name.c_str(), value.i);
							break;
						case  MaterialResource::EShaderFieldType::FLOAT:
							shader->SetFloat(value.name.c_str(), value.f);
							break;
						case  MaterialResource::EShaderFieldType::VEC2:
							shader->SetVec2(value.name.c_str(), value.v2);
							break;
						case  MaterialResource::EShaderFieldType::VEC3:
							shader->SetVec3(value.name.c_str(), value.v3);
							break;
						case  MaterialResource::EShaderFieldType::VEC4:
							shader->SetVec4(value.name.c_str(), value.v4);
							break;
                        case MaterialResource::EShaderFieldType::TEXT:
                            {
                            	if(value.text.isUsingComputeGeneratedTex)
	                            {
		                            if(value.text.Compute == nullptr)
			                            continue;

		                            shader->SetInt(value.name.c_str(), textID);
		                            value.text.Compute->BindShader();
		                            value.text.Compute->SetFloat("_GlobalTime", Time::GlobalTime());
		                            uint Texture = value.text.Compute->Dispatch();
		                            value.text.Compute->UnbindShader();
		                            glActiveTexture(GL_TEXTURE0+ textID);
		                            glBindTexture(GL_TEXTURE_2D, Texture);
		                            ++textID;
	                            }
                            	else
	                            {
		                            if(value.text.text == nullptr)
			                            continue;

		                            shader->SetInt(value.name.c_str(), textID);
		                            value.text.text->BindTexture(textID);
		                            ++textID;
	                            }
                                break;
                            }
						default:
							break;
					}
				}

				if(_anim)
				{ shader->SetAnim(_anim); }

				shader->SetFloat("_GlobalTime", Time::GlobalTime());
				shader->SetMVP(_tr, _cam);
				shader->SetLights(_cam);
			}
		}

		glBindVertexArray(subMesh.VAO);
		glBindBuffer(GL_ARRAY_BUFFER, subMesh.VBO);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,subMesh.EBO);
		glDrawElements(GL_TRIANGLES, subMesh.numOfIndices,GL_UNSIGNED_INT, nullptr);

		if(shader != nullptr)
		{
            for(auto& value : mat->fields)
            {
                if(value.type == MaterialResource::EShaderFieldType::TEXT)
                {
                	if(value.text.isUsingComputeGeneratedTex)
	                {
		                int TexUnit = 0;
		                shader->GetInt(value.name.c_str(), &TexUnit);
		                glActiveTexture(GL_TEXTURE0+ TexUnit);
		                glBindTexture(GL_TEXTURE_2D, 0);
	                }
                	else
	                {
		                if(value.text.text == nullptr)
			                continue;

		                int TexUnit = 0;
		                shader->GetInt(value.name.c_str(), &TexUnit);
		                value.text.text->UnBindTexture(TexUnit);
	                }

                }
            }
		}
	}
	glBindVertexArray(0);
}


#ifndef ARRAYSIZE
#define ARRAYSIZE(arr) (sizeof(arr) / sizeof(arr[0]))
#endif

GL::Shader::Shader(ShaderResource *_s) :IShader(EResourceType::Shader)
{
    name = _s->name;
    source = _s;
    std::vector<char*> tab;
	tab.push_back(_s->VertexSource.data());
	ShaderWrapper vShader = CreateShader(GL_VERTEX_SHADER, 1, tab);
	tab.clear();
	tab.push_back(_s->FragSource.data());
	ShaderWrapper fShader = CreateShader(GL_FRAGMENT_SHADER, 1, tab);

	if(vShader.error || fShader.error)
	{
		if(vShader.error)
		{
			GLchar infoLog[1024];
			glGetShaderInfoLog(vShader.id, ARRAYSIZE(infoLog), nullptr, infoLog);
			Log::Send(infoLog, Log::ELogSeverity::ERROR);
		}
		if(fShader.error)
		{
			GLchar infoLog[1024];
			glGetShaderInfoLog(fShader.id, ARRAYSIZE(infoLog), nullptr, infoLog);
			Log::Send(infoLog, Log::ELogSeverity::ERROR);
		}
		return;
	}


	ProgID = glCreateProgram();
	glAttachShader(ProgID, vShader.id);
	glAttachShader(ProgID, fShader.id);
	glLinkProgram(ProgID);
	GLint linkStatus;
	glGetProgramiv(ProgID, GL_LINK_STATUS, &linkStatus);
    vert = vShader.id;
    frag = fShader.id;
	if (linkStatus == GL_FALSE)
	{
		GLchar infoLog[1024];
		glGetProgramInfoLog(ProgID, ARRAYSIZE(infoLog), nullptr, infoLog);
		Log::Send(infoLog, Log::ELogSeverity::ERROR);
		//TODO : cleanup at return
	}

	LoadShaderFields();
}

GL::ComputeShader::ComputeShader(ComputeShaderResource *_cs) : ICompute(EResourceType::Compute)
{
	shader.name = _cs->name;
	ICompute::name = _cs->name;
    //use binaries if available
	std::vector<char*> tab;
	source = _cs;
	tab.push_back(_cs->ComputeSource.data());
	Shader::ShaderWrapper computeW = shader.CreateShader(GL_COMPUTE_SHADER, 1, tab);
	if(computeW.error)
		return;
	shader.ProgID = glCreateProgram();
	glAttachShader(shader.ProgID, computeW.id);
	glLinkProgram(shader.ProgID);
	compute = computeW.id;
	GLint linkStatus;
	glGetProgramiv(shader.ProgID, GL_LINK_STATUS, &linkStatus);
	if (linkStatus == GL_FALSE)
	{

		GLchar infoLog[1024];
		glGetProgramInfoLog(shader.ProgID, ARRAYSIZE(infoLog), nullptr, infoLog);
		Log::Send(infoLog, Log::ELogSeverity::ERROR);
		//TODO : Cleanup at return
		computeW.error = true;
	}
	LoadShaderFields();
	ReloadFields();

}

void GL::ComputeShader::InitTex(Vec2i size)
{
	if(isInit)
	{
		if(TexSize.x == size.x && TexSize.y == size.y)
			return;
		TexSize = size;
		shader.BindShader();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, OutTexId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, TexSize.x, TexSize.y, 0, GL_RGBA, GL_FLOAT,
		             NULL);
		glBindImageTexture(0, OutTexId, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
		shader.UnbindShader();
		return;
	}
	TexSize = size;
	shader.BindShader();
	glGenTextures(1, &OutTexId);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, OutTexId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, TexSize.x, TexSize.y, 0, GL_RGBA, GL_FLOAT,
	             NULL);
	glBindImageTexture(0, OutTexId, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	glBindTexture(GL_TEXTURE_2D,0);
	isInit =true;
	shader.UnbindShader();
}

uint GL::ComputeShader::Dispatch()
{
	if(Dispatched)
		return OutTexId;

	return ForceDispatch();
}

uint GL::ComputeShader::ForceDispatch()
{
	Dispatched = true;
	uint textID = 1;
	for(auto& value : ComputeFields)
	{
		switch (value.type)
		{
			case EShaderFieldType::BOOL:
				SetBool(value.name.c_str(), value.b);
				break;
			case  EShaderFieldType::INT:
				SetInt(value.name.c_str(), value.i);
				break;
			case  EShaderFieldType::FLOAT:
				SetFloat(value.name.c_str(), value.f);
				break;
			case  EShaderFieldType::VEC2:
				SetVec2(value.name.c_str(), value.v2);
				break;
			case  EShaderFieldType::VEC3:
				SetVec3(value.name.c_str(), value.v3);
				break;
			case  EShaderFieldType::VEC4:
				SetVec4(value.name.c_str(), value.v4);
				break;
			case EShaderFieldType::TEXT:
			{
				if(value.text.isUsingComputeGeneratedTex)
				{
					if(value.text.Compute == nullptr)
						continue;

					SetInt(value.name.c_str(), textID);
					value.text.Compute->BindShader();
					value.text.Compute->SetFloat("_GlobalTime", Time::GlobalTime());
					uint Texture = value.text.Compute->Dispatch();
					value.text.Compute->UnbindShader();
					glActiveTexture(GL_TEXTURE0+ textID);
					glBindTexture(GL_TEXTURE_2D, Texture);
					++textID;
				}
				else
				{
					if(value.text.text == nullptr)
						continue;

					SetInt(value.name.c_str(), textID);
					value.text.text->BindImageTexture(textID);
					++textID;
				}
				break;
			}
			default:
				break;
		}
	}
	glDispatchCompute((GLuint)512, (GLuint)512, 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	for(auto& value : ComputeFields)
	{
		if(value.type == EShaderFieldType::TEXT)
		{
			if(value.text.isUsingComputeGeneratedTex)
			{
				int TexUnit = 0;
				GetInt(value.name.c_str(), &TexUnit);
				glActiveTexture(GL_TEXTURE0+ TexUnit);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
			else
			{
				if(value.text.text == nullptr)
					continue;

				int TexUnit = 0;
				GetInt(value.name.c_str(), &TexUnit);
				value.text.text->UnBindImageTexture(TexUnit);
			}

		}
	}
	return OutTexId;
}

void GL::ComputeShader::ReloadShader()
{
	std::vector<char*> fragS;
	fragS.push_back(source->ComputeSource.data());
	glShaderSource(compute,1,fragS.data(), nullptr);
	glCompileShader(compute);
	GLint success = 0;
	glGetShaderiv(compute, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		GLchar infoLog[1024];
		glGetShaderInfoLog(compute, 1024, nullptr, infoLog);
		Log::Send(infoLog, Log::ELogSeverity::ERROR);
	}
	if (success > 0)
	{
		glLinkProgram(shader.ProgID);
		LoadShaderFields();
		ReloadFields();
		fs::path p=ResourcesLoader::SolidPath.parent_path();
		for(auto& elt : source->path)
		{
			p.append(elt);
		}
		fs::path vert = p;

		vert.append(source->name).append("main.Compute");
		p.append(source->name + ".SCompute");
		std::vector<char> buffer;

		source->ToDataBuffer(buffer);
		std::ofstream file(p, std::fstream::binary | std::fstream::trunc);
		std::ofstream vertfile(vert, std::fstream::binary | std::fstream::trunc);

		if(file.is_open())
		{
			file.write(buffer.data(),buffer.size());
			file.close();
		}
		if(vertfile.is_open())
		{
			vertfile.write(source->ComputeSource.data(),source->ComputeSource.size());
			vertfile.close();
		}

	}
}

void GL::ComputeShader::BindShader()
{
	shader.BindShader();
}

void GL::ComputeShader::UnbindShader()
{
	shader.UnbindShader();
}

void GL::ComputeShader::SetFloat(const char *_name, float _value)
{
	shader.SetFloat(_name,_value);
}

void GL::ComputeShader::SetInt(const char *_name, int _value)
{
	shader.SetInt(_name,_value);
}

void GL::ComputeShader::SetBool(const char *_name, bool _value)
{
	shader.SetBool(_name,_value);
}

void GL::ComputeShader::SetVec2(const char *_name, Vec2 _value)
{
	shader.SetVec2(_name,_value);
}

void GL::ComputeShader::SetVec3(const char *_name, Vec3 _value)
{
	shader.SetVec3(_name,_value);
}

void GL::ComputeShader::SetVec4(const char *_name, Vec4 _value)
{
	shader.SetVec4(_name,_value);
}

void GL::ComputeShader::SetMatrix(const char *_name, Mat4<float> _value)
{
	shader.SetMatrix(_name,_value);
}

void GL::ComputeShader::SetFloatArray(const char *_name, int size, float *_value)
{
	shader.SetFloatArray(_name,size,_value);
}

void GL::ComputeShader::SetIntArray(const char *_name, int size, int *_value)
{
	shader.SetIntArray(_name,size,_value);
}

void GL::ComputeShader::SetVec3Array(const char *_name, int size, Vec3 *_value)
{
	shader.SetVec3Array(_name,size,_value);
}

void GL::ComputeShader::SetMatrixArray(const char *_name, int size, Mat4<float> *_value)
{
	shader.SetMatrixArray(_name,size,_value);
}

void GL::ComputeShader::GetIntArray(const char *_name, int size, int *_value)
{
	shader.GetIntArray(_name,size,_value);
}

void GL::ComputeShader::GetInt(const char *_name, int *_value)
{
	shader.GetInt(_name,_value);
}

void GL::ComputeShader::LoadShaderFields()
{
	shader.LoadShaderFields();
}

std::vector<ShaderUniform> &GL::ComputeShader::GetUniformList()
{
	return shader.GetUniformList();
}

void GL::ComputeShader::SetMVP(Transform &_model, Camera &_camera) const
{
	shader.SetMVP(_model,_camera);
}

void GL::ComputeShader::SetLights(Camera &_camera) const
{
	shader.SetLights(_camera);
}

void GL::ComputeShader::SetAnim(Animation *_anim) const
{
	shader.SetAnim(_anim);
}

void GL::ComputeShader::SetMaterial(const char *_name)
{
	shader.SetMaterial(_name);
}

std::string &GL::ComputeShader::GetFragSource()
{
	return shader.GetFragSource();
}

std::string &GL::ComputeShader::GetVertSource()
{
	return shader.GetVertSource();
}

void GL::ComputeShader::SetFragSource(const std::string &_src)
{
	shader.SetFragSource(_src);
}

void GL::ComputeShader::SetVertSource(const std::string &_src)
{
	shader.SetVertSource(_src);
}

void GL::ComputeShader::SetComputeSource(const std::string &_src)
{
	source->ComputeSource = _src;
}

std::string &GL::ComputeShader::GetComputeSource()
{
	return source->ComputeSource;
}

Vec2i GL::ComputeShader::GetTexSize()
{
	return TexSize;
}


//+Type = GL_VERTEX_SHADER / GL_FRAGMENT_SHADER / GL_COMPUTE_SHADER
GL::Shader::ShaderWrapper GL::Shader::CreateShader(GLenum _type, int _sourceCount, std::vector<char *> &_sources)
{
	ShaderWrapper compute;
	compute.id = glCreateShader(_type);

	glShaderSource(compute.id, _sourceCount, _sources.data(), nullptr);
	glCompileShader(compute.id);
	GLint compileStatus;
	glGetShaderiv(compute.id, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus == GL_FALSE)
	{
		GLchar infoLog[1024];
		glGetShaderInfoLog(compute.id, 1024, nullptr, infoLog);
		Log::Send(infoLog, Log::ELogSeverity::ERROR);
		compute.error = true;
	}

	return compute;
}

void GL::Shader::ReloadShader()
{
    std::vector<char*> fragS;
    std::vector<char*> vertS;
    fragS.push_back(source->FragSource.data());
    vertS.push_back(source->VertexSource.data());
    glShaderSource(frag,1,fragS.data(), nullptr);
    glShaderSource(vert,1,vertS.data(), nullptr);
    glCompileShader(frag);
    glCompileShader(vert);
    GLint success = 0;
    glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        GLchar infoLog[1024];
        glGetShaderInfoLog(frag, 1024, nullptr, infoLog);
        Log::Send(infoLog, Log::ELogSeverity::ERROR);
    }
    success = 0;
    glGetShaderiv(vert, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        GLchar infoLog[1024];
        glGetShaderInfoLog(vert, 1024, nullptr, infoLog);
	    Log::Send(infoLog, Log::ELogSeverity::ERROR);
    }
    if (success > 0)
    {
        glLinkProgram(ProgID);
        LoadShaderFields();
		fs::path p=ResourcesLoader::SolidPath.parent_path();
		for(auto& elt : source->path)
		{
		    p.append(elt);
		}
	    fs::path vert = p;
	    fs::path frag = p;
	    vert.append(source->name).append("main.vert");
	    frag.append(source->name).append("main.frag");
		p.append(source->name + ".SVertFrag");
		std::vector<char> buffer;

		source->ToDataBuffer(buffer);
		std::ofstream file(p, std::fstream::binary | std::fstream::trunc);
	    std::ofstream vertfile(vert, std::fstream::binary | std::fstream::trunc);
	    std::ofstream fragfile(frag, std::fstream::binary | std::fstream::trunc);

	    if(file.is_open())
		{
			file.write(buffer.data(),buffer.size());
			file.close();
		}
	    if(vertfile.is_open())
	    {
		    vertfile.write(source->VertexSource.data(),source->VertexSource.size());
		    vertfile.close();
	    }
	    if(fragfile.is_open())
	    {
		    fragfile.write(source->FragSource.data(),source->FragSource.size());
		    fragfile.close();
	    }
    }
}

void GL::Shader::SetFloat(const char *_name, float _value)
{
	glUseProgram(ProgID);
	glUniform1f(glGetUniformLocation(ProgID,_name), _value);
}

void GL::Shader::SetMVP(Transform& _model, Camera& _camera)const
{

	glUseProgram(ProgID);

	glUniformMatrix4fv(glGetUniformLocation(ProgID,"proj"),1,GL_FALSE,_camera.GetProjection().elements.data());
	glUniformMatrix4fv(glGetUniformLocation(ProgID,"view"),1,GL_FALSE,_camera.GetView().elements.data());
	Mat4<float> modelM =  (_model.GetGlobalMatrix()) ;
	glUniformMatrix4fv(glGetUniformLocation(ProgID,"model"),1,GL_FALSE,modelM.elements.data());

}

void GL::Shader::SetLights(Camera& _camera) const
{
    glUseProgram(ProgID);

    std::vector<LightData> lights = Light::GetLightList();
    int i = 0;
    for(const auto& light : lights)
    {
        std::string id = std::to_string(i);
        Mat4<float> tf =light.light->gameObject->transform->GetGlobalMatrix();

        Vec3 pos = Vec3(tf.elements[12],tf.elements[13],tf.elements[14]);
        glUniform1i(glGetUniformLocation(ProgID,std::string("_lights[" + id + "].type").c_str()),light.light->type);
        glUniform3fv(glGetUniformLocation(ProgID,std::string("_lights[" + id + "].pos").c_str()),1,&pos.x);
        glUniform3fv(glGetUniformLocation(ProgID,std::string("_lights[" + id + "].color").c_str()),1,&light.light->color.x);
        glUniform1f(glGetUniformLocation(ProgID,std::string("_lights[" + id + "].intensity").c_str()),light.light->intensity);
        if(light.light->type == Light::DIRECTIONAL)
            glUniform3fv(glGetUniformLocation(ProgID,std::string("_lights[" + id + "].dir").c_str()),1,&light.light->dir.x);

        ++i;
    }
    glUniform1i(glGetUniformLocation(ProgID,"_nbLights"),lights.size());
    glUniform3fv(glGetUniformLocation(ProgID,"_camPos"),1,&_camera.position.x);
}

void GL::Shader::SetAnim(Animation* _anim) const
{
    if(_anim == nullptr)
        return;
    glUseProgram(ProgID);

    //for(int)
    std::vector<Mat4f> transforms = _anim->GetFinalTrans();
    auto a = _anim->GetAnim();
    if(a== nullptr)
	    return;
    glUniform1i(glGetUniformLocation(ProgID,"MAX_BONES"),a->numOfBones);

    for (int i = 0; i < transforms.size(); ++i)
    {
        std::string id = std::to_string(i);
        glUniformMatrix4fv(glGetUniformLocation(ProgID,std::string("finalBonesMatrices[" + id + "]").c_str()),1,GL_TRUE,transforms[i].elements.data());

    }
}

void GL::Shader::SetMaterial(const char *_name)
{

}

void GL::Shader::SetInt(const char *_name, int _value)
{
	glUseProgram(ProgID);
	glUniform1i(glGetUniformLocation(ProgID,_name), _value);
}

void GL::Shader::SetBool(const char *_name, bool _value)
{
	glUseProgram(ProgID);
	glUniform1i(glGetUniformLocation(ProgID,_name), _value);
}

void GL::Shader::SetMatrix(const char *_name, Mat4<float> _value)
{
	glUseProgram(ProgID);
	glUniformMatrix4fv(glGetUniformLocation(ProgID,_name),1,GL_FALSE, _value.elements.data());
}

void GL::Shader::SetFloatArray(const char *_name, int size, float *_value)
{
	glUseProgram(ProgID);
	glUniform1fv(glGetUniformLocation(ProgID,_name),size, _value);
}

void GL::Shader::SetIntArray(const char *_name, int size, int *_value)
{
	glUseProgram(ProgID);
	glUniform1iv(glGetUniformLocation(ProgID,_name),size, _value);
}



void GL::Shader::SetVec3Array(const char *_name, int size, Vec3 *_value)
{
	glUseProgram(ProgID);
	glUniform3fv(glGetUniformLocation(ProgID,_name),size, &(_value->x));
}

void GL::Shader::SetMatrixArray(const char *_name, int size, Mat4<float> *_value)
{
	glUseProgram(ProgID);
	glUniformMatrix4fv(glGetUniformLocation(ProgID,_name),size,GL_FALSE, _value->elements.data());
}

void GL::Shader::SetVec3(const char *_name, Vec3 _value)
{
	glUseProgram(ProgID);
	glUniform3f(glGetUniformLocation(ProgID,_name), _value.x,_value.y,_value.z);
}

void GL::Shader::SetVec2(const char *_name, Vec2 _value)
{
	glUseProgram(ProgID);
	glUniform2f(glGetUniformLocation(ProgID,_name), _value.x,_value.y);
}

void GL::Shader::SetVec4(const char *_name, Vec4 _value)
{
	glUseProgram(ProgID);
	glUniform4f(glGetUniformLocation(ProgID,_name), _value.x,_value.y,_value.z, _value.w);
}


void GL::Shader::GetIntArray(const char *_name, int size, int *_value)
{
	glGetnUniformiv(ProgID, glGetUniformLocation(ProgID,_name), size, _value);
}

void GL::Shader::GetInt(const char *_name, int *_value)
{
	int loc = glGetUniformLocation(ProgID,_name);
	if(loc == -1)
		return;
	glGetUniformiv(ProgID, loc, _value);
	GLenum error = glGetError();
    while (error != GL_NO_ERROR)
    {
        const unsigned char* err =glGetString(error);
        if(err != nullptr)
            std::cout << err << "\n";
        error = glGetError();
    }
}

void GL::Shader::LoadShaderFields()
{
    uniforms.clear();
    int count = -1;
    glGetProgramiv(ProgID, GL_ACTIVE_UNIFORMS,&count);

    for (int i = 0; i < count; ++i)
    {
        ShaderUniform shaderUniform;
        int name_len=-1, num=-1;
        GLenum type = GL_ZERO;
        char name[100];
        glGetActiveUniform(ProgID, GLuint(i), sizeof(name)-1,
                            &name_len, &num, &type, name );
        name[name_len] = 0;
        //GLuint location = glGetUniformLocation(ProgID, name );
        if(name[0] == '_')
            continue;

        shaderUniform.name = name;
        switch (type)
        {
            case GL_BOOL:
                shaderUniform.type = MaterialResource::EShaderFieldType::BOOL;
                break;
            case GL_INT:
                shaderUniform.type = MaterialResource::EShaderFieldType::INT;
                break;
            case GL_FLOAT:
                shaderUniform.type = MaterialResource::EShaderFieldType::FLOAT;
                break;
            case GL_FLOAT_VEC2:
                shaderUniform.type = MaterialResource::EShaderFieldType::VEC2;
                break;
            case GL_FLOAT_VEC3:
                shaderUniform.type = MaterialResource::EShaderFieldType::VEC3;
                break;
            case GL_FLOAT_VEC4:
                shaderUniform.type = MaterialResource::EShaderFieldType::VEC4;
                break;
            case GL_SAMPLER_2D:
                shaderUniform.type = MaterialResource::EShaderFieldType::TEXT;
                break;
	        case GL_IMAGE_2D:
		        shaderUniform.type = MaterialResource::EShaderFieldType::TEXT;
		        break;
		    default:
                shaderUniform.type = MaterialResource::EShaderFieldType::NONE;
                break;
        }

        uniforms.push_back(shaderUniform);
    }

}

std::vector<ShaderUniform> &GL::Shader::GetUniformList()
{
    return uniforms;
}

GL::Texture::Texture(ImageResource *_image)
{
	//Log::Send("IMAGE CHAN NUM = " + std::to_string(_image->ChannelsNum));
	name = _image->name;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//if(_image->ChannelsNum == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, _image->x, _image->y, 0, GL_RGBA, GL_UNSIGNED_BYTE, _image->image.data());
	//else if(_image->ChannelsNum == 3)
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, _image->x, _image->y, 0, GL_RGB, GL_UNSIGNED_BYTE, _image->image.data());
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

std::string& GL::Shader::GetFragSource()
{
    return source->FragSource;
}

std::string& GL::Shader::GetVertSource()
{
    return source->VertexSource;
}

void GL::Shader::SetFragSource(const std::string& _src)
{
    source->FragSource = _src;
}

void GL::Shader::SetVertSource(const std::string& _src)
{
    source->VertexSource = _src;
}

void GL::Shader::UnbindShader()
{
	glUseProgram(LastShader);
	LastShader = 0;
}

void GL::Shader::BindShader()
{
	glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*)&LastShader);
	glUseProgram(ProgID);
}

void GL::Texture::BindTexture(uint _texUnit)
{
	glActiveTexture(GL_TEXTURE0+ _texUnit);
	glBindTexture(GL_TEXTURE_2D, texId);
}

void GL::Texture::UnBindTexture(uint _texUnit)
{
	glActiveTexture(GL_TEXTURE0+ _texUnit);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void GL::Texture::BindImageTexture(uint _texUnit)
{
	if(_texUnit == 0)
		return;
	glBindImageTexture(_texUnit, texId, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);

}

void GL::Texture::UnBindImageTexture(uint _texUnit)
{
	if(_texUnit == 0)
		return;
	glBindImageTexture(_texUnit, 0, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
}

GL::Cubemap::Cubemap(CubemapResource *_cubemap)
{
    name = _cubemap->name;

    glGenTextures(1, &cubemapID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapID);

    for (size_t i = 0; i < 6; ++i)
    {
        const CubemapResource::CubemapImg& cubemapImg = _cubemap->imageMap[i];
        GLenum format = (cubemapImg.ChannelsNum == 3) ? GL_RGB : GL_RGBA;
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, cubemapImg.x, cubemapImg.y, 0, format, GL_UNSIGNED_BYTE, cubemapImg.image.data());
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void GL::Cubemap::BindCubemap()
{
    glBindTexture(GL_TEXTURE_CUBE_MAP,cubemapID);
}

void GL::Cubemap::UnBindCubemap()
{
    glBindTexture(GL_TEXTURE_CUBE_MAP,0);
}
