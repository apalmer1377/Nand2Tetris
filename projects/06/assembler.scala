import scala.io.Source

class Assembler(filename: String) {
  val processed: Array[String] = {
    Source.fromFile(filename).getLines.map(line => { 
      if (!(line.trim() == "") || (line.length() >= 2 && line.substring(0,2) == ("//")) ) 
        line.split("//")(0).trim()
      else
        ""
    }).filter(line => line != "").toArray
  }
  val symbols: Map[String,Int] = {
    val nonMachine = processed.filter(line => line.contains("("))
    bindSymbols(processed.map(line => {
      if (line.contains("@") && processed.indexOf(line.replace("@","(") + ")") == -1)
        mapAt(line.replace("@",""),-(processed.indexOf(line)+1))
      else if (line.contains("("))
        mapAt(line.replace("(","").replace(")",""),processed.indexOf(line)-nonMachine.indexOf(line))
      else
        ("",-1)
    }).toMap-"")

  }
  
  val machineCode: Array[String] = {
    processed.map(line => {
      if (line.contains("@"))
        "0" + ((util.Try(line.replace("@","").toInt).getOrElse(symbols(line.replace("@","")))) + 32768).toBinaryString.substring(1)
      else if (!line.contains("("))
        CCommand(line)
      else
        ""
    }).filter(line => line != "")
  }
  
  def CCommand(line: String): String = {
    "111" + (
    if (line.contains("="))
      (if (line.split("=")(1).contains("M")) "1" else "0") + ALU(line.split("=")(1)) + DEST(line.split("=")(0)) + "000"
    else if (line.contains(";"))
      "0" + ALU(line.split(";")(0)) + "000" + (line.split(";")(1) match {
        case "JGT" => "001"
        case "JEQ" => "010"
        case "JGE" => "011"
        case "JLT" => "100"
        case "JNE" => "101"
        case "JLE" => "110"
        case "JMP" => "111"   
      })
    else
      ALU(line) + "000"
    )
  }
  
  def ALU(command: String): String = {
    command.replace("M","A").replace("A+D","D+A") match {
      case "0" => "101010"
      case "1" => "111111"
      case "-1" => "111010"
      case "D" => "001100"
      case "A" => "110000"
      case "!D" => "001101"
      case "!A" => "110001"
      case "-D" => "001111"
      case "-A" => "110011"
      case "D+1" => "011111"
      case "A+1" => "110111"
      case "D-1" => "001110"
      case "A-1" => "110010"
      case "D+A" => "000010"
      case "D-A" => "010011"
      case "A-D" => "000111"
      case "D&A" => "000000"
      case "D|A" => "010101"
    }
  }
  
  def DEST(dest: String): String = {
    (if (dest.contains("A")) "1" else "0") +
    (if (dest.contains("D")) "1" else "0") +
    (if (dest.contains("M")) "1" else "0")
  }
  
  def bindSymbols(table: Map[String,Int]): Map[String,Int] = {
    val filtab = table.toSeq.filter((row: (String,Int)) => row._2 < 0).sortBy(_._2).reverse
    table.map((row: (String,Int)) => {
      if (row._2 < 0)
        (row._1,16+filtab.indexOf(row))
      else
        row
    }).toMap
  }
  
  def mapAt(symbol: String, pos: Int = -1): (String, Int) = {
    symbol match {
      case "SP" => (symbol,0)
      case "LCL" => (symbol,1)
      case "ARG" => (symbol,2)
      case "THIS" => (symbol,3)
      case "THAT" => (symbol,4)
      case "SCREEN" => (symbol,16384)
      case "KBD" => (symbol,24576)
      case _ => mapGeneral(symbol,pos)
    }
  }
  
  def mapGeneral(symbol: String,pos: Int = -1): (String, Int) = {
    if (symbol.startsWith("R") && util.Try(symbol.replace("R","").toInt).getOrElse(16) < 16 ) 
      (symbol, symbol.replace("R","").toInt)
    else if (util.Try(symbol.toInt).toOption.isEmpty)
      (symbol, pos)
    else
      ("",-1)
  }
}

/*object Test {
    def main(args: Array[String]) = {
      val test = "/Users/austinpalmer/Desktop/GitHub/Nand2Tetris/projects/04/mult/mult.asm"
      val t = new Assembler(test)
      t.Load("/Users/austinpalmer/Desktop/GitHub/Nand2Tetris/projects/04/mult/mult.asm").foreach(println)
      println("da fuq")
    }
}*/

import java.io._
val assemble = new Assembler(args(0))
val writer = new PrintWriter(new File(args(0).replace(".asm",".hack")))
assemble.machineCode.foreach(line => writer.write(line + "\n"))
writer.close()