require File.expand_path('../../spec_helper', __FILE__)

describe "DateTime#downto" do

  it "should be able to step backward in time" do
    ds    = DateTime.civil(2000, 4, 14)
    de    = DateTime.civil(2000, 3, 29)
    count = 0
    ds.downto(de) do |d|
      d.should <= ds
      d.should >= de
      count += 1
    end
    count.should == 17
  end

  it "should respect fractional days" do
    ds    = DateTime.civil(2000, 4, 14, 0)
    de    = DateTime.civil(2000, 3, 29, 12)
    count = 0
    ds.downto(de) do |d|
      d.should <= ds
      d.should >= de
      count += 1
    end
    count.should == 16

    ds    = DateTime.civil(2000, 4, 14, 12)
    de    = DateTime.civil(2000, 3, 29, 0)
    count = 0
    ds.downto(de) do |d|
      d.should <= ds
      d.should >= de
      count += 1
    end
    count.should == 17
  end

end
