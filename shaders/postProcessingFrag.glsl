#version 330

in vec2 textureCoords;

out vec4 FragColor;

uniform sampler2D screenTexture;

const float offset = 1.0 / 300.0;

const int kernelSize = 9;

vec4 CalculateAverage(vec2 cornerOffset, int segmentSize);
float CalcStdDeviation(vec4 average, vec2 cornerOffset, int segmentSize);
int FindSmallest(float elements[4]);

void main()
{
    int segmentSize = (kernelSize / 2);
    vec2 cornerOffset1 = vec2(-offset * float(kernelSize / 2), -offset * float(kernelSize / 2));
    // vec4 avg1 = CalculateAverage(cornerOffset1, segmentSize);
    // float stdDv1 = CalcStdDeviation(avg1, cornerOffset1, segmentSize);

    vec2 cornerOffset2 = vec2(-offset * float(kernelSize / 2), 0);
    // vec4 avg2 = CalculateAverage(cornerOffset2, segmentSize);
    // float stdDv2 = CalcStdDeviation(avg2, cornerOffset2, segmentSize);

    vec2 corneroffset3 = vec2(0, -offset * float(kernelSize / 2));
    // vec4 avg3 = CalculateAverage(corneroffset3, segmentSize);
    // float stdDv3 = CalcStdDeviation(avg3, corneroffset3, segmentSize);

    vec2 corneroffset4 = vec2(0, 0);
    // vec4 avg4 = CalculateAverage(corneroffset4, segmentSize);
    // float stdDv4 = CalcStdDeviation(avg4, corneroffset4, segmentSize);

    vec4 colors[] = {
        CalculateAverage(cornerOffset1, segmentSize),
        CalculateAverage(cornerOffset2, segmentSize),
        CalculateAverage(corneroffset3, segmentSize),
        CalculateAverage(corneroffset4, segmentSize)
    };

    float stdDevs[] = {
        CalcStdDeviation(colors[0], cornerOffset1, segmentSize),
        CalcStdDeviation(colors[1], cornerOffset2, segmentSize),
        CalcStdDeviation(colors[2], corneroffset3, segmentSize),
        CalcStdDeviation(colors[3], corneroffset4, segmentSize)
    };
    
    // FragColor = vec4(vec3(1.0 - texture(screenTexture, textureCoords)), 1.0);

    FragColor = colors[FindSmallest(stdDevs)];
}

int FindSmallest(float elements[4])
{
    float smallest = elements[0];
    int smallIndex = 0;
    for(int i = 1; i < 4; i++)
    {
        if(elements[i] < smallest)
        {
            smallest = elements[i];
            smallIndex = i;
        }
    }
    return smallIndex;
}

vec4 CalculateAverage(vec2 cornerOffset, int segmentSize)
{
    vec4 sum = vec4(0);
    for(int i = 0; i < segmentSize; i++)
    {
        for(int j = 0; j < segmentSize; j++)
        {
            sum += texture(screenTexture, vec2(textureCoords.s + (cornerOffset.x + float(i) * offset), textureCoords.t + (cornerOffset.y + float(j) * offset)));
        }
    }
    return sum / (segmentSize * segmentSize);
}

float CalcStdDeviation(vec4 average, vec2 cornerOffset, int segmentSize)
{
    float sum = 0;
    for(int i = 0; i < segmentSize; i++)
    {
        for(int j = 0; j < segmentSize; j++)
        {
            float avgValue = average.x + average.y + average.z + average.w;
            vec4 currentColor = texture(screenTexture, vec2(textureCoords.s + (cornerOffset.x + float(i) * offset), textureCoords.t + (cornerOffset.y + float(j) * offset)));
            float currentValue = currentColor.x + currentColor.y + currentColor.z + currentColor.w;
            sum += pow(avgValue - currentValue, 2.0);
        }
    }

    return sqrt(sum / (segmentSize * segmentSize));
}