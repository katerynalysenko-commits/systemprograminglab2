package main
import "fmt"

// коментар

#dummy_preprocessor

func main() {
    var x int = 42
    y := 3.14
    z := 0x2A
    s := "hello\n"
    c := 'A'

    result := (x + int(y)) * z / 2

    if result > 100 && y != 0.0 {
        fmt.Println("big result", result)
    } else {
        fmt.Println("small result", result)
    }

    for i := 0; i < 5; i++ {
        fmt.Println(i)
    }
}
