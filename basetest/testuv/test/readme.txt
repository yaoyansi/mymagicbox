1. 打开场景, source下面的mel函数
proc selectTheOverlapFaces(string $sgName)
{
    select -cl;
    string $faces[] = `peltOverlap $sgName`;
    select -cl;
    for($f in $faces)
    {
        select -tgl $f;
    }    
}

2. 这个命令以shadeing group名字为输入参数, 例如:"initialShadingGroup"
执行:
selectTheOverlapFaces("initialShadingGroup");
与shading group相连的mesh的uv如果有穿插, 那些面片就会被选择

