# ShaderState
Of course PADrend allows you to add custom shaders to nodes. This is done my using the so called `ShaderState`. This state contains a shader program, which is automatically used when the corresponding node is rendered. Furthermore it is possible to add uniform variables to the state. Before rendering those uniforms are send to the GPU.

The usage of the `ShaderState` can be explained best by simply describing a simple example.

## Simple Example
This example will basically render a simple quad with a chess texture. But this time the color value of each fragment is not only based on the texture, but also on the value of the uv coordinate.
The final result will look like this:

![Colored chess texture](ColoredChess.png)

Before we start with the shader, we first have to create a simple mesh. In this case we just build a simple quad. Afterwards we create a corresponding `GeometryNode` and add the chess texture to it.
```
static Vec2 = Geometry.Vec2;
static Vec3 = Geometry.Vec3;

var buildMesh = fn() {
	// First we build a simple Mesh, consisting of a single quad
	var mb = new Rendering.MeshBuilder();
	mb.color(new Util.Color4f(1,1,1,1));
	// Vertex 0:
	mb.position(new Vec3(0,0,0));
	mb.texCoord0(new Vec2(0,1));
	mb.addVertex();

	// Vertex 1:
	mb.position(new Vec3(10,0,0));
	mb.texCoord0(new Vec2(1,1));
	mb.addVertex();

	// Vertex 2:
	mb.position(new Vec3(10,10,0));
	mb.texCoord0(new Vec2(1,0));
	mb.addVertex();

	// Vertex 3:
	mb.position(new Vec3(0,10,0));
	mb.texCoord0(new Vec2(0,0));
	mb.addVertex();

	// create quad
	mb.addQuad(0,1,2,3);
	// return mesh
	return mb.buildMesh();
};

// build GeometryNode with corresponding mesh
var geo = new MinSG.GeometryNode(buildMesh());
// create chess texture of size 1024*1024, with tiles of side length 64
var chess = Rendering.createChessTexture(1024, 1024, 64);
// create new TextureState
var texState = new MinSG.TextureState(chess);
// you could also set the TextureUnit:
texState.setTextureUnit(0); // only needed if you add more than one texture though...
// add state to node
geo += texState;
```
If you have read the Texturing tutorial, this code is straight forward.
Next we continue with the actual shader code.

### Shader code
```
var vertexShaderCode = "
void main(void) {
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = ftransform();
}
";
var fragmentShaderCode = "
uniform sampler2D chessTexture;

void main(void) {
	vec2 uv = gl_TexCoord[0].st;
	vec4 result = texture2D(chessTexture, uv);
	result.r *= uv.s;
	result.g *= uv.t;
	gl_FragColor = result;
}
";
var shader = Rendering.Shader.createShader(vertexShaderCode, fragmentShaderCode);
```

Our vertex shader is super simple, it does nothing more than just setting the texture coordinate and the position. This is done by using OpenGL internal functions. The fragment shader has a bit more functionality. It first retrieves the uv coordinate from the `gl_TexCoord` array. Afterwards we just look up the color of the chess texture at the given uv coordinate. To make it look different from the default shader, we further multiply the `r` and `g` channels of the color with the uv coordinate. This will result in a blue/purple coloring of the white tiles. The last step is just setting the `gl_FragColor` value.

### ShaderState
Now we use this shader programm to instantiate a new `ShaderState`. Furthermore we also set a uniform variable. In this case we only have a single uniform value, which is the `chessTexture` variable. It is a `sampler2D` and therefore we have to set the correct texture unit for it.
```
var shaderState = new MinSG.ShaderState(shader);
shaderState.setUniform("chessTexture", Rendering.Uniform.INT, [0]);
```
The `TextureState` was set to texture unit 0, therefore we also use the same unit here. After that, we can finally add this state to the node:
```
geo += shaderState;
```

Now the node will be rendered using the given shader.