#pragma once

#include "Rendering/renderer.hpp"
#include <glad/glad.h>

namespace Solid
{
    // RC: Rendering architecture?
    class SOLID_API OpenGL45Renderer : public Renderer
    {
    private:
        OpenGL45Renderer();
        ~OpenGL45Renderer() override = default;
        //TODO: CLEAN CODE (c'est sale oui)
        uint gridProgram;
        uint grid_vbo;
        uint grid_ebo;
        uint linesProgram;
        uint lines_vbo;
        uint lines_ebo;
        uint skyboxProgram;
        uint skybox_vbo;
        uint skybox_vao;
        uint skybox_ebo;

        void InitGridGL();
        void InitLinesGL();
        void InitSkyboxGL();

    public:
        static Renderer* InitRenderer();


        static void GLDebugCallback(GLenum _source, GLenum _type, GLuint _id, GLenum _severity, GLsizei _length,
                                    const GLchar* _message, const void* _userParam);

        void Clear(const Vec2i& _windowSize) const final;
        void ClearColor(const Vec4& _clearColor) const final;
        [[nodiscard]]
        Framebuffer CreateFramebuffer(const Vec2i& _size) const final;

        void UpdateFramebuffer(const Framebuffer& _framebuffer) const final;

        void BeginFramebuffer(Framebuffer &_framebuffer) final;
        void EndFramebuffer()  final;
        ShaderBinary GetShaderBinary(uint _PID) const final;
        uint CreateShaderFromBinary(ShaderBinary _binary) const final;

	    void DrawSolidGrid(const Camera& _camera, float _gridSize, Vec3 _color, float _thickness) const final;
	    void DrawSkybox(const Camera &_camera) const final;
	    void DrawLines(const Camera& _camera, std::vector<Vec3> _points, std::vector<uint> indices) const final;

	    virtual void viewport(Vec2 pos, Vec2 Ratio) override;
    };

} //!namespace




