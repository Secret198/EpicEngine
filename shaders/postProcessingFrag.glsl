#version 330

in vec2 textureCoords;

out vec4 FragColor;

uniform sampler2D screenTexture;

const float offset = 1.0 / 300.0;

const int kernelSize = 9;

vec3 CalculateAverage(vec2 cornerOffset, int segmentSize);
float CalcStdDeviation(vec3 average, vec2 cornerOffset, int segmentSize);

void main()
{
    vec3 avg1 = CalculateAverage(vec2(-offset * (kernelSize / 2), -offset * (kernelSize / 2)), (kernelSize / 2));
    float stdDv1 = CalcStdDeviation(avg1);

    vec3 avg2 = CalculateAverage(-offset * (kernelSize / 2), 0, (kernelSize / 2));
    float stdDv2 = CalcStdDeviation(avg2);

    vec3 avg3 = CalculateAverage(0, -offset * (kernelSize / 2)), (kernelSize / 2);
    float stdDv3 = CalcStdDeviation(avg3);

    vec3 avg4 = CalculateAverage(0, 0, (kernelSize / 2));
    float stdDv4 = CalcStdDeviation(avg4);

    // FragColor = texture(screenTexture, textureCoords);
    FragColor = vec4(vec3(1.0 - texture(screenTexture, textureCoords)), 1.0);
}

vec3 CalculateAverage(vec2 cornerOffset, int segmentSize)
{
    vec3 sum = 0;
    for(int i = 0; i < segmentSize; i++)
    {
        for(int j = 0; j < segmentSize; j++)
        {
            sum += texture(screenTexture, vec2(textureCoords.s + (cornerOffset.x + i * offset), textureCoords.t + (cornerOffset.y + j * offset)));
        }
    }
    return sum / (segmentSize * segmentSize);
}

float CalcStdDeviation(vec3 average, vec2 cornerOffset, int segmentSize)
{
    vec3 sum = 0;
    for(int i = 0; i < segmentSize; i++)
    {
        for(int j = 0; j < segmentSize; j++)
        {
            sum += pow((average - texture(screenTexture, vec2(textureCoords.s + (cornerOffset.x + i * offset), textureCoords.t + (cornerOffset.y + j * offset)))), 2.0);
        }
    }

    return sqrt(sum / (segmentSize * segmentSize));
}